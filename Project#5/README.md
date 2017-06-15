## Project#5 2016004011 with SQlite & Cocos2d-x
***
# 프로젝트 소개
이 프로젝트는 cocos2d-x의 c++언어로 짜여진 테트리스 게임이다
테트리스의 거의 원형게임형태를 가지고 있지만 추가적으로 보너스 게임이 있다는 점이 이 프로젝트의 특이한 점이라 할 수 있다
보너스 게임으로 2048이라는 블럭 보드 게임을 추가하였고 이 2048이라는 게임은 다른 사람이 만든 오픈소스 프로젝트에서 소스코드를 가져와 적용한 것이다

# 간단 메뉴얼
1.	프로그램을 실행시킨다
2.	메인 메뉴창의 ‘New Game’버튼을 클릭한다(소리를 끄고 싶으면 Sound:On을 클릭한다)
3.	게임을 진행한다
4.	떨어지는 블럭을 왼쪽으로 이동하고 싶으면 키보드에서 왼쪽 모양 화살표를 누른다,반대로 오른쪽이동은 오른쪽 화살표키이다
5.	블럭을 조금 빨리 떨어지게 하고 싶으면 아래방향화살표키를 누른다, 그러면 강제로 1칸이 추가적으로 떨어진다
6.	블럭을 떨어질 수 있을때까지 한번에 떨어트리고 싶으면 엔터키를 누른다
7.	도형을 왼쪽으로 90도 회전하고 싶으면 스페이스바를 클릭한다
8.	현재 도형과 다음에 떨어질 도형의 순서를 바꾸고 싶으면 shift키(오른쪽,왼쪽상관없음)를 누른다
9.	게임도중 일시정지나 소리를 추가적으로 키고 끄거나 게임을 포기하고 싶으면 왼쪽 상단의 메뉴탭 모양 버튼을 클릭하거나 esc버튼을 눌러 메뉴를 호출한다
10.	보너스 게임이 뜨게 되면 오른쪽 상태 창에 goal이라는 숫자를 화살표키들로 숫자블럭을 움직여 제작한다(게임을 포기하고 싶으면 Give up game 버튼을 클릭하되 클릭하면 테트리스 점수에 패널티가 가해진다)
11.	도형이 더 이상 떨어질수없는 상태가 되면 게임 오버가 되고 화면에 뜨는 재시작 버튼이나 메인화면으로 돌아가기 버튼을 클릭해 게임을 다시 시작하거나 메인화면으로 돌아간다
12.	이전에 행했던 게임의 전적기록을 보고 싶으면 메인 메뉴창으로 나와 ScoreBoard버튼을 클릭한다
13.	ScoreBoard창에서 나오고 싶으면 오른쪽 하단의 전원버튼 모양의 버튼을 클릭한다

## Algorithm
![algorithm](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%235/cocosproj/Tetris/algorithm.png)


## Sample  
![main](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%201.23.48.png)
![maingame](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%208.03.33.png)
![resumegame](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%208.03.41.png)
![record](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%201.29.39.png)
![2048game](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%201.24.32.png)
![endgame](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%2010.12.32.png)

### **구현한 기능**
1. 메인메뉴 배경화면 추가  
2. 메인메뉴 창 추가   
3. 메인메뉴에서 게임 혹은 프로그램의 소리를 끄고 킬수 있도록함   
4. 이전에 한 테트리스들의 기록을 DB에 저장  
5. 기록한 테트리스 결과값들을 DB에서 불러와 기록창에 순위를 매겨 보여줌  
6. 게임을 일시정지/일시정지해제 할수 있도록 짬  
7. 게임을 여러 유저가 참여 할 수 있도록 user객체를 사용함  
8. 게임도중 메뉴를 누를 수 있도록함  
9. 게임도중 소리를 키고 끌 수 있게함   
10. 게임도중 메인메뉴 창으로 나갈 수 있게함  
11. 다음 블럭이 무엇인지 미리 볼수 있도록함   
12. 점수 시스템을 추가함   
13. 게임의 레벨을 추가함   
14. 게임을 시작한 이래로 경과한 시간을 보여줌  
15. 점수가 현재 얼마인지 보여줌  
16. 일정 점수가 쌓이면 보너스게임을 할 수 있도록함  
17. 현재까지 지운 줄의 수를 보여줌   
18. 현재 레벨을 gui 창에보여줌   
19. 테트리스판의 줄을 연속적으로 제거하면 콤보를 얻고 이를 통해 추가적 콤보점수를 얻을 수 있음  
20. 게임도중 엔터키를 누르면 떨어지는 블럭이 내려갈 수 있는 곳까지 한번에 내려갈수 있게함  
21. 게임도중 화살표 아래키를 누르면 일정속도로 떨어지는 블럭이 강제로 한칸씩 내려올수 있게함  
22. shift키를 누르면 다음 블럭과 현재 블럭이 바뀔 수 있도록함  
23. 떨어지는 블럭에 색깔을 입힘  
24. 떨어지는 블럭이 랜덤으로 생성되게함  
25. space 바(bar)를 누르면 도형이 반시계 방향으로 90도씩 회전하게함  
26. 화살표 오른쪽 왼쪽키를 누르게 되면 도형이 왼쪽과 오른쪽으로 이동할 수 있게함  
27. 블럭을 움직이고 회전할때 발생하는 효과음을 추가   
28. 게임을 진행중일때, 메인 메뉴창을 로딩할때 , 보너스게임할때 , 게임오버됐을때 bgm이 나오도록함  

### ref
    www.sqlite.org  
    https://github.com/CodeLadyJJY/2048 
