# STM32_Elevator
STM32를 활용한 RTOS 엘리베이터 프로젝트

![](https://velog.velcdn.com/images/jimeaning/post/c5593d26-ab7c-4fe6-90f5-8a852f933a7b/image.jpeg)


## 프로젝트 소개

Intel AI SW 아카데미에서 한 달 동안 배운 내용을 바탕으로 세탁기를 만드는 프로젝트

-   **프로젝트 기간**  : 24.1.22 ~ 24.1.25
-   **프로젝트 인원**  : 개인 프로젝트
-   **프로젝트 기능**
	- 1층부터 4층까지 구성되어 있다.
	- 문 닫힘 버튼을 누르면 엘리베이터가 동작한다.
	- 현재 머무르는 층보다 가고자 하는 층이 더 위면 엘리베이터는 올라간다.
	- 현재 머무르는 층보다 가고자 하는 층이 더 아래면 엘리베이터는 내려간다.
	- 꼭대기 층에 가면 조도 센서가 인식되어 LCD에 출력하고 더이상 올라가지 않는다.
	- 1층 포토센서가 감지되면 LCD에 출력하고 더이상 내려가지 않는다.

## 설계 프로세스
![](https://velog.velcdn.com/images/jimeaning/post/40539a9b-972d-461b-beb1-3516f74418e5/image.png)

## FSM
![](https://velog.velcdn.com/images/jimeaning/post/f82c74e6-0bca-4997-b69e-02495c177894/image.png)

## Sequence Diagram
![](https://velog.velcdn.com/images/jimeaning/post/5dc33a12-9974-45d3-b03e-d1f06cfc08e3/image.png)

## 사용한 모듈과 역할
![](https://velog.velcdn.com/images/jimeaning/post/e1ea9883-cb6c-46db-9615-aad4ca729a21/image.png)
![](https://velog.velcdn.com/images/jimeaning/post/cd902d83-ce2d-4c18-b42a-c3c0872fca3b/image.png)

## Output
![](https://velog.velcdn.com/images/jimeaning/post/2cdabd00-6120-448c-ba38-1418364c3892/image.png)
