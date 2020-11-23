# LineTracking
Line Tracking using OpenCV <br>
start: 2020.11.13

# Specification
* OpenCV 4.4.0
* Microsoft Visual C++ 2019

# Global Variables
- queue&lt;Point&gt; lftq1 : 왼쪽 차선의 좌표1
- queue&lt;Point&gt; lftq2 : 왼쪽 차선의 좌표2
- queue&lt;Point&gt; rghtq1 : 오른쪽 차선의 좌표2
- queue&lt;Point&gt; rghtq2 : 오른쪽 차선의 좌표2


<br>
lftq1과 lftq2의 같은 인덱스의 Point는 한 라인을 이룬다.
  



# Functions
## __0. Mat IncreaseContrast(Mat img) <br>__
- 히스토그램 평활화

## __0.1 Mat changeColor(Mat img, int color) <br>__
- int color: 어떤 색을 추출할지. 0: blue, 1: white
- 특정 색만 추출하여 흰색으로 만들고 나머지는 검정으로 만들어 줌
- 구현: blue만 되어 있음


## __1. Mat makeROI(Mat& img, int type, int makeGray = 1) <br>__
- int type : 리턴할 이미지를 어떻게 할지. 0: gray, 1: 전체 평활화, 2: 하단부만 평활화
- 흑백 변환
- type에 따라 대비 늘리기: 수행 IcreaseContrast 함수 사용
- 가우시안필터로 블러링


## __1.5 Mat hideNonROI(Mat& imgROI, int shape) <br>__
 - int shape: ROI 영역의 모양. 0: 아래쪽 절반, 1: 삼각형, 2: 사다리꼴
 - imgROI에서 ROI 영역의 모양 부분만 남기고 전부 black으로 바꾼다.

## __2. vector&lt;Point&gt; findEdgeLines(Mat original, Mat img, int lowThres = 100, int highThres = 300, const char* windowName = "fel img") <br>__
  - int threshold : 허프라인스p에서 사용할 vote 수
  - 리턴: 찾은 라인들의 좌표. 2개씩 짝지어서 한 라인을 이룬다.
  - __캐니 엣지__ : 엣지 찾기
  - __HoughLinesP__ : 라인 찾기
    + 곡선 찾기

## __3. vector&lt;Point&gt; find2Line(vector&lt;Point&gt; lines, int cols) <br>__
   - vector&lt;Point&gt; lines: findEdgeLines의 결과.
   - 여러 라인 중 차선으로 지정할 라인 찾기
      + ~~거리가 가까운 라인끼리 클러스터 이룬 후 그 중 평균을 구한다.~~
      + lines의 size가 4개 미만이면 전부 Point(0,0)으로 채워진 벡터 리턴
      + 이미지 기준 왼쪽에 있으면 왼쪽 라인으로, 오른쪽에 있으면 오른쪽 라인으로 취급
      + 왼쪽 라인중 2개, 오른쪽 라인중2개를 선택
      + 선택 기준: 
   - 리턴: left line, right line의 좌표들. 2개씩 짝지어서 한 라인을 이룬다.
      + 왼쪽1, 왼쪽2, 오른쪽1, 오른쪽2가 들어있다.
   
   - 

## __4. void addNewPoint( vector&lt;Point&gt; lines) <br>__
  - find2Line 결과로 찾은 라인이 있는 경우 실행됨
  - 포인트가 (0, 0)이 아니면 해당되는 큐에 포인트 넣고 하나 pop한다.
    + 해당되는 큐는 lines의 인덱스에 따라 정해져 있다.

## 5. vector&lt;Point&gt; findMeanLine() <br>
   - 지금까지의 10개 평균점으로 라인 찾기
   - 큐에서 찾은 point들을 빼서 평균값을 구한다.

## 6. Mat drawLines(Mat img, vector&lt;Point&gt; lines) <br>
- 원본 영상에 찾은 차선 표시
  
## 7. int main(int argc, char** argv) <br>
- 비디오 영상 프레임별로 불러오기
- 이미지 리사이즈 (작게)
- changeColor로 영상에서 blue 부분 찾고, 원본 영상에 노란색으로 표시해주기-> bluecopy
- makeROI, hideNonROI로 ROI 만들기 -> imgROI
- findEdgeLines로 라인 찾고 find2Line으로 거르기
- addNewPoint, findeMeanLine으로 최종 라인 결정
- drawLines로 최종 라인 

 
# Trials
## 1. Mat service(vector&lt;Point&gt; lines) <br>
- 차선 이탈시 표시
- 미구현

## 2. Mat houghLineImage(Mat img, Mat imgROI, vector&lt;Point&gt; red_lines)
- 

## 3. vector&lt;Point&gt; find2Line(vector&lt;Point&gt; lines, int cols)
  

# Expansion
* __앞에 장애물 생긴 경우__
  - 장애물이 생겼는지 어떻게 알까??
  - 장애물이 생기기 전 지금까지의 20개 평균으로 찾기
* __차선이 사라진 경우__
  - 차선이 사라지기 전 지금까지의 20개 평균으로 찾기
  

# Roles
function 0, 1 &&  PPT 제작 : __YEJIN-LILY<br>__
function 2, 4, 5, 6, 7 : __LeeJE20<br>__
function 3, 시연 영상, 발표: __KimRiun<br>__


# Schedule
11/13 : function 0, 1 구현

11/20 : ppt 제작
  
