# SE-2017(2017CSE2019) 오픈소스개발

## Project#3 2016004011 with MySQL
***
## Sample
![sample1](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/sample1.png)
![sample2](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/sample2.png)
![s3](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202017-04-30%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%2011.51.22.png)
![s4](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202017-04-30%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%2011.51.55.png)
![s5](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202017-04-30%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%2011.52.21.png)
![s6](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202017-04-30%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%2011.52.57.png)
![s7](https://github.com/hsh97Erica/SE-2017/blob/master/Project%233/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202017-04-30%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%2011.53.12.png)

![s8](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202017-05-01%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%202.17.44.png)
![s9](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202017-05-01%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%202.18.00.png)
![s10](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/스크린샷%202017-05-01%20오후%2011.38.45.png)
![s11](https://github.com/hsh97Erica/SE-2017/blob/master/Project%233/converted_file_368e8e30.gif?raw=true)
### **구현한 기능**
* 새로운 노트 쓰기 및 저장
* 저장한 노트 수정하고 저장하기
* 저장한 노트 삭제하기
* 저장한 노트 불러오기
* 저장한 노트의 제목을 추가
* 기본적인 에디터 라이브러리의 모든 기능추가(링크, 동영상,사진,음악을 링크로 가져오기 같은 기능포함)
* 저장한 노트 목록 필터링(검색어가 포함된 메모를 검색)
* 대문창 동적인 배경화면 추가(with WebGL)
* 현재시간
* 로그인
* 회원가입
* 회원가입시 이미 등록된 아이디가 존재하는 지 중복체크
* 회원 정보 변경(현재 비밀번호, 닉네임)
* 현재 시간 폰트변경
* 현재 시간 배경추가
* 네이버 실시간 검색어 파싱(node.js)
* 실시간 검색어를 파싱한 데이터를 polymer로 json형식으로 가져와 유저에게 보여줌
* 한양대(portal rss) 공지사항 파싱(node.js)
* 공지사항을 파싱한 데이터를 polymer에서 가져와 표로 유저에게 보여줌
* 공지사항을 약 6시간 주기로 새로고침함(페이지 재로딩x)
* 날씨 추가
* (로그인없는)로컬 메모장 구현
* base64로 임시 통신 암호화
* 로그아웃
* 메모 에디터에서 동영상추가(url) - 예정
* 메모 에디터에서 사진추가(with base64) - 예정
* 실시간 검색어 창의 배경을 둥글게 만듬
* 현재시간을 띄워주는 라이브러리에서 내 앱에 맞게 시계(초,분,시간, 날짜등) 배치를 변경함
* 실시간 검색어가 순차적으로 보이게함(애니메이션 포함 약 3초 주기)
* 실시간 검색어를 약 30초 주기로 받아옴
* 각각의 메모장 비밀번호 걸기기능 구현
* 검색어가 순차적으로 보이게 할 수 있도록 라이브러리를 수정(neon-text-board 의 랜덤순서를 순차적으로 볼수 있게 수정)
* 학사 공지사항의 제목을 클릭시 해당하는 공지사항을 볼 수 있도록 함
* 폴리머로 로그인 창을 제작함
* 타이틀바 사용자가 원하는 색상으로 변경하도록 함
* 일렉트론앱 적용
* 필터링가능하고 목록을 카테고리별로 구별할수있는 리스트뷰를 라이브러리를 조합해 제작
* 메모목록에서 저장소별로 메모를 구분 지을 수 있도록 카테고리 구분자를 카테고리명에 따라 쓸 수 있도록 라이브러리를 조합해 또다른 임시 라이브러리를 제작함
## ref
    [polymer 1.0](https://www.polymer-project.org/1.0/docs/devguide/feature-overview)
    [webcomponents](https://www.webcomponents.org)
    [node.js](https://nodejs.org/ko/)
    [mysql](https://www.mysql.com)
    https://github.com/PolymerElements/paper-input
	https://www.webcomponents.org/element/petja/paper-searchbox
    https://www.webcomponents.org/element/ryanburns23/paper-weather/demo/demo/index.html
    https://www.webcomponents.org/element/petja/paper-userpicture
    https://www.webcomponents.org/element/PolymerElements/paper-card
    https://www.webcomponents.org/element/PolymerElements/iron-flex-layout/demo/demo/index.html
    https://www.webcomponents.org/element/wincinderith/paper-divider/paper-divider
    https://www.webcomponents.org/element/PolymerElements/iron-flex-layout/demo/demo/index.html
    and etc
    list of ref names
    polymer_components
        accessibility-developer-tools
        app-layout
        app-localize-behavior
        app-route
        async
        chai
        digital-clock
        font-roboto
        fs-behavior
        fs-client
        fs-person-behavior
        fs-person-card
        fs-person-portrait
        fs-person-summary
        hydrolysis
        import-tinymce
        intl-messageformat
        iron-a11y-announcer
        iron-a11y-keys
        iron-a11y-keys-behavior
        iron-ajax
        iron-autogrow-textarea
        iron-behaviors
        iron-checked-element-behavior
        iron-collapse
        iron-component-page
        iron-data-table
        iron-doc-viewer
        iron-dropdown
        iron-fit-behavior
        iron-flex-layout
        iron-form-element-behavior
        iron-icon
        iron-icons
        iron-iconset-svg
        iron-image
        iron-input
        iron-list
        iron-localstorage
        iron-location
        iron-media-query
        iron-menu-behavior
        iron-meta
        iron-overlay-behavior
        iron-pages
        iron-resizable-behavior
        iron-scroll-target-behavior
        iron-selector
        iron-validatable-behavior
        js-base64
        js-md5
        keyboardevent-key-polyfill
        lodash
        marked
        marked-element
        mocha
        moment
        moment-element
        neon-animation
        neon-text-board
        paper-avatar-behavior
        paper-badge
        paper-behaviors
        paper-button
        paper-card
        paper-checkbox
        paper-dialog
        paper-dialog-behavior
        paper-dialog-scrollable
        paper-divider
        paper-dropdown-menu
        paper-fab
        paper-header-panel
        paper-icon-button
        paper-input
        paper-item
        paper-listbox
        paper-material
        paper-menu-button
        paper-more-button
        paper-radio-button
        paper-radio-group
        paper-ripple
        paper-search
        paper-searchbox
        paper-spinner
        paper-styles
        paper-toolbar
        paper-tooltip
        paper-userpicture
        paper-weather
        polymer
        polymer-quill
        polymer-tinymce
        prism
        prism-element
        promise-polyfill
        quill
        selection-mgr
        selectionchange-polyfill
        sinon-chai
        sinonjs
        stacky
        test-fixture
        tinymce
        undo
        undo-mgr
        web-animations-js
        web-component-tester
        webcomponentsjs
        wysiwyg-e


## Project#5 2016004011 with SQlite & Cocos2d-x
***
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
