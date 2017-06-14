## Project#5 2016004011 with SQlite & Cocos2d-x
***
## Sample  
![main](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%201.23.48.png)
![maingame](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%208.03.33.png)
![resumegame](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%208.03.41.png)
![record](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%201.29.39.png)
![2048game](https://raw.githubusercontent.com/hsh97Erica/SE-2017/3eb1238d1f8d9017d02af8b860b8bebd5cc161fd/Project%235/cocosproj/Tetris/sampleshot/스크린샷%202017-06-14%20오후%201.24.32.png)


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
