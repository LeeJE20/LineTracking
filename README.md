# LineTracking
Line Tracking using OpenCV
start: 2020.11.13

# Specification
* OpenCV 4.4.0
* Microsoft Visual C++ 2019

# Global Variables
- queue<Point> p1 : 차선의 좌표1
- queue<Point> p2 : 차선의 좌표2
<br>
p1과 p2의 같은 인덱스의 Point는 한 라인을 이룬다.
  



# Functions
## __0. Mat IncreaseContrast(Mat img) <br>__
- 히스토그램 평활화

## __1. Mat makeROI(int shape = 0) <br>__
- int shape: ROI 영역의 모양. 0: 아래쪽 절반, 1: 삼각형, 2: 사다리꼴
- 영상에서 한 프레임씩 불러오기
- ROI 만들기: 관심 영역 제외 나머지는 black
- 흑백 변환
- 대비 늘리기: IcreaseContrast 함수 사용
- 가우시안필터로 블러링

## __2. vector<Point> findEdgeLines(Mat img, int threshold) <br>__
  - int threshold : 허프라인스p에서 사용할 vote 수
  - 리턴: 찾은 라인들의 좌표. 2개씩 짝지어서 한 라인을 이룬다.
  - 캐니 엣지
  - 허프라인스p
    + 곡선 찾기

## __3. vector<Point> find2Line(vector<Point> lines) <br>__
   - vector<Point> lines: findEdgeLines의 결과.
   - 여러 라인 중 차선으로 지정할 라인 찾기
      + 거리가 가까운 라인끼리 클러스터 이룬 후 그 중 평균을 구한다.
   - 리턴: left line, right line의 좌표. 2개씩 짝지어서 한 라인을 이룬다.

## __4. void addNewPoint( vector<Point> lines) <br>__
  - find2Line 결과로 찾은 라인이 있는 경우 실행됨
  - p1과 p2의 맨 앞 원소 삭제를 2회 반복
  - p1과 p2의 맨 뒤에 새로 찾은 point 추가 (2회 반복)

## __5. vector<Point> findMeanLine() <br>__
   - find2Line 결과로 찾은 라인이 없는 경우 실행됨
   - 차선이 사라지기 전 지금까지의 20개 평균으로 찾기

## __6. Mat drawLines(vector<Point> lines) <br>__
    - 원본 영상에 찾은 차선 표시
  
## _7. Mat service(vector<Point> lines) <br>__
    - 차선 이탈시 표시
  

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
  
