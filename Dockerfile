FROM ubuntu:jammy

WORKDIR /app/ci_cd_test

RUN apt-get update && apt-get install -y \
    gcc \
    nano \
    vim \
    build-essential \
    cmake \
    git \
    rm -rf /var/lib/apt/lists/*


COPY src/ src/
COPY tests/ tests/

COPY Dockerfile docker-compose.yml .github/workflows/ci.yml ./

RUN g++ src/app.cpp -o app_executable
RUN g++ tests/test.cpp -o test_executable

