pipeline{
    agent any

    parameters{
        booleanParam(name: 'RUN_LINT', defaultValue: true, description: 'Do you want to run linting?')
        booleanParam(name: 'RUN_TESTS', defaultValue: true, description: 'Do you want to run tests?')        
        booleanParam(name: 'PUSH_TO_DOCKER_HUB', defaultValue: false, description: 'Do you want to push image to Docker Hub?')
   
        //TODO
        choice(name: 'DEPLOY_ENV', choices: ['dev', 'staging', 'prod'], description: 'Select the deployment environment')
    }

    options{
        timeout(time: 30, unit: 'MINUTES')

        disableConcurrentBuilds()

        buildDiscarder(logRotator(numToKeepStr: '10',
                                  artifactNumToKeepStr: '10',
                                  daysToKeepStr: '365'))

        timestamps()
    }


    environment{
        DOCKER_COMPOSE_FILE = 'docker-compose-ci.yml'
        DOCKER_IMAGE_NAME = 'george0506/jenkins-learning'
        DOCKER_IMAGE_TAG = 'latest'
    }



    stages{

        stage('Initialize pipeline'){
            steps{
                    echo 'Initializing pipeline...'
                    echo 'Pipeline initialized successfully.'
                    2>&1 | tee docker_info.log
                    sh "docker version"
                    sh "docker compose version"
                    sh "which docker"

            }

            post{
                always{
                    archiveArtifacts artifacts: 'docker_info.log', allowEmptyArchive: true
                }
            }
        }


        stage('Build an image'){
            steps{
                echo 'Starting build of C++ project...'
                sh "docker build -t ${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG} ."
                echo 'Docker image have been built successfully.'
            }
        }

        stage('Parrallel lint and test'){
           parallel{
                stage('Linting'){
                    
                    when{ expression  {params.RUN_LINT} }

                    environment{
                        COMPOSE_PROJECT_NAME = 'lint-${env.BUILD_ID}'
                    }

                    steps{
                        echo 'Linting...'
                        sh "docker compose -f ${DOCKER_COMPOSE_FILE} up --exit-code-from cppcheck 2>&1 | tee linting.log"
                        echo 'Linting completed successfully.'
                    }

                    post{
                        always{
                            archiveArtifacts artifacts: 'linting.log', allowEmptyArchive: true
                            
                            // clean parallel container to avoid conflicts
                            sh "docker compose -p ${COMPOSE_PROJECT_NAME} -f ${DOCKER_COMPOSE_FILE} down"
                        }
                    }
                }

                stage('Testing'){

                    when{ expression {params.RUN_TESTS} }

                    environment{
                        COMPOSE_PROJECT_NAME = 'test-${env.BUILD_ID}'
                    }
                    steps{
                        echo 'Testing...'
                        // concatanate stdout and stderr in one stream and create log file 
                        sh "docker compose -f ${DOCKER_COMPOSE_FILE} up --exit-code-from ci_unit_tests 2>&1 | tee unit_tests.log"
                        echo 'Testing completed successfully.'
                    }

                    post{
                        always{
                            //archive to Jenkins interface
                            archiveArtifacts artifacts: 'unit_tests.log', allowEmptyArchive: true
                            sh "docker compose -p ${COMPOSE_PROJECT_NAME} -f ${DOCKER_COMPOSE_FILE} down"
                        }
                    }
                }
            }
        }

        stage('Build source code'){
            steps{
                echo 'Building source code...'
                sh "docker compose -f ${DOCKER_COMPOSE_FILE} up --exit-code-from ci_app"
                echo 'Source code built successfully.'
            }
        }

        stage('Push to Docker Hub'){
            environment{
                    DOCKER_REGISTRY_ENTRY_DATA = credentials('docker-hub-creds')
            }

            when{
                allOf{
                    expression{
                        return params.PUSH_TO_DOCKER_HUB
                    }
                    branch 'main'               
                }
            }

            options{
                timeout(time: 5, unit: 'MINUTES')
            }
                
            steps{
                echo 'Logging into Docker Hub...'
                script{
                    retry(2){
                        try{
                            //give pass via pipe to avoid showing in logs
                            sh "echo \${DOCKER_REGISTRY_ENTRY_DATA_PSW} | docker login -u \${DOCKER_REGISTRY_ENTRY_DATA_USR} --password-stdin"
                            echo 'Logged into Docker Hub successfully.'
                        }      
                        catch (err){
                            echo "It seems that you use incorrect credentials: ${err.getMessage()}"
                            throw err
                        }
                    }
                }
                    echo 'Pushing image to Docker Hub...'
                    sh "docker push ${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}"
                    echo 'Image pushed to Docker Hub successfully.'
            }
        }
        
        stage('Pipeline duration'){
            steps{
                script{
                    echo "Pipeline duration: ${currentBuild.durationString}."
                }
            }
        }
    }


    post{
        
        always{            
            echo 'Pipeline was completed with status ${currentBuild.currentResult}.'

            // send message to mine telegram bot 
            withCredentials([
                string(credentialsId: 'telegram-bot-id', variable: 'TG_BOT_TOKEN'),
                string(credentialsId: 'telegram-bot-chat-id', variable: 'TG_CHAT_ID')])
            {
                sh """
                    curl -s -X POST https://api.telegram.org/bot${TG_BOT_TOKEN}/sendMessage \
                    -d chat_id=${TG_CHAT_ID} \
                    -d text="Pipeline was ended with status ${currentBuild.currentResult}. Duration: ${currentBuild.durationString}."
                """
            }
        }
        cleanup{
            echo 'Cleaning up...'
            sh "docker compose --file ${DOCKER_COMPOSE_FILE} down"
            sh 'docker logout'
            cleanWs() // clear jenkins workspace
        }


    }

}
    

 