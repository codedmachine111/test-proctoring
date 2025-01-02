>[!NOTE]
>This project was built as a learning exercise to experiment with the V4L2 pipeline and OpenCV.

# Test proctoring

A lightweight proctoring software designed to monitor terminal-based quiz exams. This project demonstrates the implementation of a proctoring application using C++.

## Installation steps

1. - Fork the [repo](https://github.com/codedmachine111/test-proctoring)
- Clone the fork to your local machine `git clone <URL>`
- Change current directory `cd test-proctoring`

2. Execute the runner script to install required deps and build the source code.
```bash
bash runner.sh
```

3. Paste the text file of your quiz in the `assets` dir and rename it to `text.txt`

4. Run the application:
```bash
./bin/main
```

>[!IMPORTANT]
>The camera device defaults to `/dev/video0`. You can modify it in the code according to your device.

## Installation using Docker

1. Build the docker image:
```bash
docker build -t proctoring:dev .
```

2. Run the docker container:
```bash
docker run --rm --device=/dev/video0 proctoring:dev
```
