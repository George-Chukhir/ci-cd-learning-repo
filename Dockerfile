FROM ubuntu:jammy

WORKDIR /app/ci_cd_test

RUN apt-get update && apt-get install -y \
    gcc \
    nano \
    vim \
    build-essential \
    cmake \
    git \
    libgtest-dev \ 
    && rm -rf /var/lib/apt/lists/*

RUN  cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/libgtest.a /usr/local/lib && cp lib/libgtest_main.a /usr/local/lib    

COPY src/ src/
COPY tests/ tests/

COPY Dockerfile docker-compose-ci.yml .github/workflows/ci.yml ./

RUN g++ src/app.cpp -o app_executable
RUN g++ tests/tests.cpp -o test_executable

