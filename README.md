# SE-2017(2017CSE2019) 오픈소스개발

## Project#3 2016004011
***
## Sample
![sample1](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/sample1.png)
![sample2](https://raw.githubusercontent.com/hsh97Erica/SE-2017/master/Project%233/sample2.png)


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
* 네이버 실시간 검색어 파싱
* 한양대(portal rss) 공지사항 파싱
* 공지사항을 파싱해 표로 유저에게 보여줌
* 공지사항을 약 6시간 주기로 새로고침함(페이지 재로딩x)
* 날씨 추가
* (로그인없는)로컬 메모장 구현 - 예정
* base64로 임시 통신 암호화
* 로그아웃
* 메모 에디터에서 동영상추가(url) - 예정
* 메모 에디터에서 사진추가(with base64) - 예정
* 실시간 검색어 창의 배경을 둥글게 만듬
* 현재시간을 띄워주는 라이브러리에서 내 앱에 맞게 시계(초,분,시간, 날짜등) 배치를 변경함
* 실시간 검색어가 순차적으로 보이게함(애니메이션 포함 약 3초 주기)
* 실시간 검색어를 약 30초 주기로 받아옴
* 검색어가 순차적으로 보이게 할 수 있도록 라이브러리를 수정(neon-text-board 의 랜덤순서를 순차적으로 볼수 있게 수정)
* 학사 공지사항의 제목을 클릭시 해당하는 공지사항을 볼 수 있도록 함
* 폴리머로 로그인 창을 제작함

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
