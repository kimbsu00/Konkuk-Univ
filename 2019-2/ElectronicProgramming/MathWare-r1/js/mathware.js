$(document).ready(function () {
    console.log("perfect!");
    var output_section = $(".output_section");
    var sentence = new Array(); //개행(\n)기준으로 한 문장이다. 개행이 없다면 sentence는 하나가 될 것이다.

    //키보드 입력이 들어올때마다 해줘야 할 것들
    $(".input_section>textarea").on("change keyup paste", function (event) {
        var input_section = $(this).val(); // 현재 textarea의 있는 문자열 전체
        sentence = input_section.split("\n"); //개행 기준으로 sentence 배열 생성
        checkInput($(this).val());
        var outputs = new Array(sentence.length);
        for (var i in sentence) { //sentence별로 수식 변환 진행해주면 된다.
            if (i != 0 || sentence[i].length != 0)
                outputs[i] = sentenceToML(sentence[i]);
        }//for문 끝
        output_section.empty();//입력할때마다 초기화하고 다시 보여주기

        for (var i in outputs) {
            if (outputs.length == 1 || i == outputs.length - 1) {
                output_section.append(outputs[i]);
            }
            else {   //개행 필요한 경우
                output_section.append(outputs[i]);
                output_section.append("</br>");
            }
            //console.log("outputs[" + i + "] : " + outputs[i].html());
        }
        $(".mathml_section>textarea").val(output_section.html());
    });

});

//sentenceToML은 sentence가 들어오면 mathml 문자열로 바꿔주는 함수
function sentenceToML(str) {
    var output_tag = $("<math xmlns=\"http://www.w3.org/1998/Math/MathML\"></math>");

    var P = new ParserArr(str);
    var maker = new StrMaker();
    var S = maker.Parsering1(P);
    console.log("STR : " + str);
    console.log("S : " + S);
    //공백관련 예외처리
    S = S.replace(/\mfenced /g, "mfencedd");
    S = S.replace(/\mstyle /g, "mstylee");
    S = S.replace(/\s/g, "&nbsp;");
    S = S.replace(/\mfencedd/g, "mfenced ");
    S = S.replace(/\mstylee/g, "mstyle ");
    output_tag.html(S);
    return output_tag;

    //테스트
    // output_tag.html(str);
    // return output_tag;
}


function checkInput(value) { //value가 들어왔을때, 한 문장에 100자 이상인지, 개행이 50개 이상인지 검사
    console.log("welcome to checkImput!");
    var sentence = value.split("\n");
    if (sentence.length >= 50) {//50줄 이상
        //alert("sentence.length : " + sentence.length);
        alert("한 입력창에는 50줄 이상 작성할 수 없습니다.");
        sentence = sentence.slice(0, 49);
        console.log("sentence.length : " + sentence.length);
        $(".input_section>textarea").val("");
        for (var i = 0; i < sentence.length; i++) {
            $(".input_section>textarea").val($(".input_section>textarea").val() + sentence[i]);
            if (i != sentence.length - 1)
                $(".input_section>textarea").val($(".input_section>textarea").val() + "\n");
        }
    }
    for (var i = 0; i < sentence.length; i++) {
        if (sentence[i].length >= 100) {    //100자 이상,
            //alert("sentence[" + i + "] : " + sentence[i].length);
            alert("한 줄에는 100자 이상 작성할 수 없습니다.");
            sentence[i] = sentence[i].substring(0, 99);
            $(".input_section>textarea").val("");
            for (var j = 0; j < sentence.length; j++) {
                $(".input_section>textarea").val($(".input_section>textarea").val() + sentence[j]);
                if (j != sentence.length - 1)
                    $(".input_section>textarea").val($(".input_section>textarea").val() + "\n");
            }
        }
    }
}

// 인자, 보조인자 모두 없는 연산자
const TIMES = 1002;
const CDOT = 1003;
const PM = 1004;
const DIV = 1006;
const AST = 1007;
const PI = 1008;
const INFINITY = 1009;
const SIN = 1010;
const COS = 1011;
const TAN = 1012;
const LEFTARROW = 1013;
const RIGHTARROW = 1014;
const LEFTRIGHTARROW = 1015;
const GEQ = 1016;
const LEQ = 1017;
const LN = 1018;
const ALPHA = 1019;
const BETA = 1020;
const GAMMA = 1021;
const DELTA = 1022;
const THETA = 1023;

// 인자 1개 무조건 필요
const UNDER = 2000;
const UPPER = 2001;
const ABS = 2002;
const VECTOR = 2003;

// 보조인자 0개 또는 1개
const LIMIT = 3000;
const LOG = 3001;

// 보조인자 0개 또는 2개
const INTEGRAL = 4000;

// 보조인자 0개 또는 1개 또는 2개
const SIGMA = 5000;

// 인자 0개 또는 1개, 보조인자 0개 또는 1개 (0개,1개 경우는 X)
const SQRT = 6000;

// 보조인자 2개 무조건 필요
const OVER = 7000;

const FAIL = 9999;
const searching = 10000;
const empty = 10001;
const notclosed = 10002;

class Str {
    constructor(sym, str) {
        this.symbolic = sym;
        this.fail_str = str;
        this.arr = new Array();
    }
    getSymbolic() {
        return this.symbolic;
    }
    push_str(s) {
        this.arr.push(s);
    }
}

class Node {
    constructor(key, fNum) {
        this.key = key;
        this.fNum = fNum;
        this.arr = new Array();
    }
    make_Trie(str, index, num) {
        if (str.length == index) {
            this.fNum = num;
            return;
        }
        else {
            var temp = this.Search(str.charAt(index));
            if (!temp) {
                this.arr.push(new Node(str.charAt(index), searching))
                this.arr[this.arr.length - 1].make_Trie(str, index + 1, num)
            } else {
                temp.make_Trie(str, index + 1, num);
            }
        }
    }
    Search(ch) {
        for (var i = 0; i < this.arr.length; i++) {
            if (this.arr[i].key == ch) {
                return this.arr[i];
            }
        }
        return undefined;
    }
}

class Trie {
    constructor() {
        this._fNum;
        this._search = undefined;
        this.start = new Node(FAIL, FAIL);
        this.init();
    }
    init() {
        // 인자, 보조인자 모두 없는 연산자
        this.start.make_Trie("times ", 0, TIMES);
        this.start.make_Trie("cdot ", 0, CDOT);
        this.start.make_Trie("pm ", 0, PM);
        this.start.make_Trie("div ", 0, DIV);
        this.start.make_Trie("ast ", 0, AST);
        this.start.make_Trie("pi ", 0, PI);
        this.start.make_Trie("infinity ", 0, INFINITY);
        this.start.make_Trie("sin ", 0, SIN);
        this.start.make_Trie("cos ", 0, COS);
        this.start.make_Trie("tan ", 0, TAN);
        this.start.make_Trie("leftarrow ", 0, LEFTARROW);
        this.start.make_Trie("rightarrow ", 0, RIGHTARROW);
        this.start.make_Trie("leftrightarrow ", 0, LEFTRIGHTARROW);
        this.start.make_Trie("geq ", 0, GEQ);
        this.start.make_Trie("leq ", 0, LEQ);
        this.start.make_Trie("ln ", 0, LN);
        this.start.make_Trie("alpha ", 0, ALPHA);
        this.start.make_Trie("beta ", 0, BETA);
        this.start.make_Trie("gamma ", 0, GAMMA);
        this.start.make_Trie("delta ", 0, DELTA);
        this.start.make_Trie("theta ", 0, THETA);

        // 인자 1개 무조건 필요
        this.start.make_Trie("_ ", 0, UNDER);
        this.start.make_Trie("^ ", 0, UPPER);
        this.start.make_Trie("abs ", 0, ABS);
        this.start.make_Trie("vector ", 0, VECTOR);

        // 보조인자 0개 또는 1개
        this.start.make_Trie("limit ", 0, LIMIT);
        this.start.make_Trie("limit[", 0, LIMIT);
        this.start.make_Trie("log ", 0, LOG);
        this.start.make_Trie("log[", 0, LOG);

        // 보조인자 0개 또는 2개
        this.start.make_Trie("integral ", 0, INTEGRAL);
        this.start.make_Trie("integral[", 0, INTEGRAL);

        // 보조인자 0개 또는 1개 또는 2개
        this.start.make_Trie("sigma ", 0, SIGMA);
        this.start.make_Trie("sigma[", 0, SIGMA);

        // 인자 0개 또는 1개, 보조인자 0개 또는 1개 (0개,1개 경우는 X)
        this.start.make_Trie("sqrt ", 0, SQRT);
        this.start.make_Trie("sqrt[", 0, SQRT);

        // 보조인자 2개 무조건 필요
        this.start.make_Trie("over[", 0, OVER);

    }
    search(ch) {
        if (!this._search) {
            this._search = this.start.Search(ch);
            if (!this._search)
                return FAIL;
            this._fNum = this._search.fNum;
        }
        else {
            this._search = this._search.Search(ch);
            if (!this._search)
                return FAIL;
            this._fNum = this._search.fNum;
        }

        if (this._fNum != searching)
            this._search = undefined;

        return this._fNum;
    }

}



class Parser {

    constructor(fNum, str) {
        this.fNum = fNum;
        this.str = str;
        this.fail_str = "";
        this.subFactor1 = null;
        this.subFactor2 = null;
        this.mainFactor = null;
        this.start();
    }

    cut(Tr) {
        for (var i = 0; i < this.str.length; i++) {
            var tmp = Tr.search(this.str.charAt(i));
            if (tmp == FAIL) {
                var test = Tr.search(this.str.charAt(i));
                if (test == searching) {
                    Tr._search = undefined;
                    this.fail_str = this.fail_str + this.str.substring(0, i);
                    this.str = this.str.substring(i, this.str.length);
                } else {
                    this.fail_str = this.fail_str + this.str.substring(0, i + 1);
                    this.str = this.str.substring(i + 1, this.str.length);
                }
                this.fNum = FAIL;
                return tmp;
            }
            if (tmp != searching) {
                this.fNum = tmp; // 체크용
                if (parseInt(this.fNum / 1000) == 1) {
                    this.str = this.str.substring(i + 1, this.str.length);
                } else {
                    this.str = this.str.substring(i, this.str.length);
                }
                return tmp;
            }
            if (tmp == searching && (i + 1) == this.str.length) {
                this.fNum = FAIL;
                this.fail_str = this.fail_str + this.str.substring(0, i + 1);
                this.str = this.str.substring(i + 1, this.str.length);
                return FAIL;
            }
        }
    }


    findFactor() {
        var i = 1;
        if (this.str.charAt(0) != ' ') {
            return "";
        }
        if (this.str.charAt(i) == '{') {
            var stack = new Array();
            var success = false;
            while (i <= this.str.length - 1) {
                if (this.str.charAt(i) == '}') {
                    if (stack.length == 1) {
                        success = true;
                        break;
                    }
                    if (stack.length == 0) {
                        success = false;
                        break;
                    }
                    stack.pop();
                }
                else if (this.str.charAt(i) == '{') {
                    stack.push('{');
                }
                i++;
            }
            if (success) {
                var ostr = this.str.substring(2, i);
                this.str = this.str.substring(i + 1, this.str.length);
                return ostr;
            }

        }
        var ostr = this.str.charAt(1);
        this.str = this.str.substring(2, this.str.length);
        return ostr;
    }



    findSubFactor() {
        var i = 0;
        var stack = new Array();
        var success = false;
        if (this.str.charAt(i) == '[') {

            while (i <= this.str.length - 1) {
                if (this.str.charAt(i) == ']') {
                    if (stack.length == 1) {
                        success = true;
                        break;
                    }
                    if (stack.length == 0) {
                        success = false;
                        break;
                    }
                    stack.pop();
                }
                else if (this.str.charAt(i) == '[') {
                    stack.push('[');
                }
                i++;
            }
            if (success) {
                if (i == 1) {
                    if (this.subFactor1 == null) {
                        this.subFactor1 = new ParserArr("");
                        this.subFactor1.fNum = empty;
                    }
                    else {
                        this.subFactor2 = new ParserArr("");
                        this.subFactor2.fNum = empty;
                    }
                    this.str = this.str.substring(i + 1, this.str.length);
                    return "";
                }
                var ostr = this.str.substring(1, i);
                this.str = this.str.substring(i + 1, this.str.length);
                return ostr;
            }
            else {
                if (this.subFactor1 == null) {
                    this.subFactor1 = new ParserArr("");
                    this.subFactor1.fNum = notclosed;
                }
                else {
                    this.subFactor2 = new ParserArr("");
                    this.subFactor2.fNum = notclosed;
                }
            }
        }
        return "";
    }

    parse(Tr) {
        if (this.str != "") {
            console.log(this.str);
            this.cut(Tr);
            switch (parseInt(this.fNum / 1000)) {
                case 1:
                    this.subFactor1 = null;
                    this.subFactor2 = null;
                    this.mainFactor = null;
                    break;
                case 2:
                    this.subFactor1 = null;
                    this.subFactor2 = null;
                    var tmp_s = this.findFactor();
                    if (tmp_s.length != 0) {
                        this.mainFactor = new ParserArr(tmp_s);
                    }
                    break;
                case 3:
                    var tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor1 = new ParserArr(tmp_s);
                    }
                    this.subFactor2 = null;
                    this.mainFactor = null;
                    break;
                case 4: // 보조인자가 1개인경우 걸러줘
                    var tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor1 = new ParserArr(tmp_s);
                    }
                    tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor2 = new ParserArr(tmp_s);
                    }
                    this.mainFactor = null;
                    break;
                case 5:
                    var tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor1 = new ParserArr(tmp_s);
                    }
                    tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor2 = new ParserArr(tmp_s);
                    }
                    this.mainFactor = null;
                    break;
                case 6: // 보조인자가 1개이고 인자가 0개인 경우 걸러줘
                    var tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor1 = new ParserArr(tmp_s);
                    }
                    this.subFactor2 = null;
                    tmp_s = this.findFactor();
                    if (tmp_s.length != 0) {
                        this.mainFactor = new ParserArr(tmp_s);
                    }
                    break;
                case 7:// 보조인자가 0개 거나 1개인 경우 걸러줘
                    var tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor1 = new ParserArr(tmp_s);
                    }
                    tmp_s = this.findSubFactor();
                    if (tmp_s.length != 0) {
                        this.subFactor2 = new ParserArr(tmp_s);
                    }
                    this.mainFactor = null;
                    break;
            }
        }
    }

    start() {
        var Tr = new Trie();
        this.parse(Tr);
    }
}



class ParserArr {
    constructor(str) {
        this.str = str;
        this.pArr = new Array();
        this.mkArr();
    }

    mkArr() {
        while (1) {
            if (this.str == "") {
                break;
            }
            var p = new Parser(0, this.str);
            this.pArr.push(p);
            this.str = p.str;
        }
    }

}


class StrMaker {

    constructor() {
        this.str = "";
    }

    Parsering1(ParserArr) {
        this.tmp = "";
        for (var i = 0; i < ParserArr.pArr.length; i++) {
            this.Parsering(ParserArr.pArr[i]);
        }
        return this.tmp;
    }

    Parsering(parser) {

        // 그냥 문자열
        if (parser.fNum == FAIL) {
            this.tmp = this.tmp + '<mn>';
            this.tmp = this.tmp + parser.fail_str;
            this.tmp = this.tmp + '</mn>';
        }
        // 인자, 보조인자 모두 없는 연산자
        if (parser.fNum == TIMES)
            this.tmp = this.tmp + "<mo>&#xD7;</mo>"
        if (parser.fNum == CDOT)
            this.tmp = this.tmp + "<mo>&#xB7;</mo>"
        if (parser.fNum == PM)
            this.tmp = this.tmp + "<mo>&#xB1;</mo>"
        if (parser.fNum == DIV)
            this.tmp = this.tmp + "<mo>&#xF7;</mo>"
        if (parser.fNum == AST)
            this.tmp = this.tmp + "<mo>*</mo>"
        if (parser.fNum == PI)
            this.tmp = this.tmp + "<mi mathvariant=\"normal\">&#x3C0;</mi>"
        if (parser.fNum == INFINITY)
            this.tmp = this.tmp + "<mo>&#x221E;</mo>"
        if (parser.fNum == SIN)
            this.tmp = this.tmp + "<mi>sin</mi>"
        if (parser.fNum == COS)
            this.tmp = this.tmp + "<mi>cos</mi>"
        if (parser.fNum == TAN)
            this.tmp = this.tmp + "<mi>tan</mi>"
        if (parser.fNum == LEFTARROW)
            this.tmp = this.tmp + "<mo>&#x2190;</mo>"
        if (parser.fNum == RIGHTARROW)
            this.tmp = this.tmp + "<mo>&#x2192;</mo>"
        if (parser.fNum == LEFTRIGHTARROW)
            this.tmp = this.tmp + "<mo>&#x2194;</mo>"
        if (parser.fNum == GEQ)
            this.tmp = this.tmp + "<mo>&#x2265;</mo>"
        if (parser.fNum == LEQ)
            this.tmp = this.tmp + "<mo>&#x2264;</mo>"
        if (parser.fNum == LN)
            this.tmp = this.tmp + "<mi>ln</mi>"
        if (parser.fNum == ALPHA)
            this.tmp = this.tmp + "<mi>&#x3B1;</mi>"
        if (parser.fNum == BETA)
            this.tmp = this.tmp + "<mi>&#x3B2;</mi>"
        if (parser.fNum == GAMMA)
            this.tmp = this.tmp + "<mi>&#x3B3;</mi>"
        if (parser.fNum == DELTA)
            this.tmp = this.tmp + "<mi>&#x3B4;</mi>"
        if (parser.fNum == THETA)
            this.tmp = this.tmp + "<mi>&#x3B8;</mi>"


        // 인자 1개 무조건 필요
        if (parser.fNum == UNDER) {
            if (parser.mainFactor == null) {
                this.tmp = this.tmp + "<mn>_</mn>";
            }
            else {
                this.tmp = this.tmp + "<msub><mrow/><mrow>";
                for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                    this.Parsering(parser.mainFactor.pArr[i]);
                }
                this.tmp = this.tmp + "</mrow></msub>";
            }

        }
        if (parser.fNum == UPPER) {
            if (parser.mainFactor == null) {
                this.tmp = this.tmp + "<mn>^</mn>";
            }
            else {
                this.tmp = this.tmp + "<msup><mrow/><mrow>";
                for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                    this.Parsering(parser.mainFactor.pArr[i]);
                }
                this.tmp = this.tmp + "</mrow></msup>";
            }
        }
        if (parser.fNum == ABS) {
            if (parser.mainFactor == null) {
                this.tmp = this.tmp + "<mn>abs</mn>";
            }
            else {
                this.tmp = this.tmp + "<mfenced open=\"|\"close=\"|\"><mrow>";
                for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                    this.Parsering(parser.mainFactor.pArr[i]);
                }
                this.tmp = this.tmp + "</mrow></mfenced>";
            }
        }
        if (parser.fNum == VECTOR) {
            if (parser.mainFactor == null) {
                this.tmp = this.tmp + "<mn>vector</mn>";
            }
            else {
                this.tmp = this.tmp + "<mover><mrow>";
                for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                    this.Parsering(parser.mainFactor.pArr[i]);
                }
                this.tmp = this.tmp + "</mrow><mo>&#x2192;</mo></mover>";
            }
        }

        // 보조인자 0개 또는 1개
        if (parser.fNum == LIMIT) {
            if (parser.subFactor1 == null || parser.subFactor1.fNum == empty) {
                this.tmp = this.tmp + "<munder><mi>lim</mi><mrow/></munder>";
            }
            else if (parser.subFactor1.fNum == notclosed) {
                this.tmp = this.tmp + "<munder><mi>limit</mi><mrow/></munder>";
            } else {
                this.tmp = this.tmp + "<munder><mi>lim</mi><mrow>";
                for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                    this.Parsering(parser.subFactor1.pArr[i]);
                }
                this.tmp = this.tmp + "</mrow></munder>"
            }
        }
        if (parser.fNum == LOG) {
            if (parser.subFactor1 == null || parser.subFactor1.fNum == empty) {
                this.tmp = this.tmp + "<mi>log</mi>";
            }
            else if (parser.subFactor1.fNum == notclosed) {
                this.tmp = this.tmp + "<mi>log</mi>";
            } else {
                this.tmp = this.tmp + "<msub><mi>log</mi><mrow>";
                for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                    this.Parsering(parser.subFactor1.pArr[i]);
                }
                this.tmp = this.tmp + "</mrow></msub>";
            }
        }
        // 보조인자 0개 또는 2개
        if (parser.fNum == INTEGRAL) {
            if (parser.subFactor1 == null) {
                this.tmp = this.tmp + "<mo>&#x222B;</mo>";
            }
            else if (parser.subFactor1.fNum == notclosed) {
                this.tmp = this.tmp + "<mi>integral</mi>";
            } else {
                if (parser.subFactor2 == null) {
                    this.tmp = this.tmp + "<mn>integral[</mn>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "<mn>]</mn>";
                } else if (parser.subFactor2.fNum == notclosed) {
                    this.tmp = this.tmp + "<mn>integral[</mn>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "<mn>]</mn>";
                } else {
                    this.tmp = this.tmp + "<msubsup><mo>&#x222B;</mo><mrow>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + '</mrow><mrow>';
                    for (var i = 0; i < parser.subFactor2.pArr.length; i++) {
                        this.Parsering(parser.subFactor2.pArr[i]);
                    }
                    this.tmp = this.tmp + '</mrow></msubsup>';
                }
            }
        }

        // 보조인자 0개 또는 1개 또는 2개
        if (parser.fNum == SIGMA) {
            this.tmp = this.tmp + "<mstyle displaystyle=\"true\">";
            if (parser.subFactor1 == null || parser.subFactor1.fNum == empty) {
                if (parser.subFactor2 == null || parser.subFactor2.fNum == empty) {
                    this.tmp = this.tmp + "<munder><mo>&#x2211;</mo><mrow/></munder>";
                }
                else {
                    if (parser.subFactor2.fNum == notclosed) {
                        this.tmp = this.tmp + "<mn>sigma[]</mn>";
                    } else {
                        this.tmp = this.tmp + "<munderover><mo>&#x2211;</mo><mrow></mrow><mrow>";
                        for (var i = 0; i < parser.subFactor2.pArr.length; i++) {
                            this.Parsering(parser.subFactor2.pArr[i]);
                        }
                        this.tmp = this.tmp + '</mrow></munderover>';
                    }
                }
            } else if (parser.subFactor1.fNum == notclosed) {
                this.tmp = this.tmp + "<mn>sigma</mn>";
            } else {
                if (parser.subFactor2 == null || parser.subFactor2.fNum == empty) {
                    this.tmp = this.tmp + "<munder><mo>&#x2211;</mo><mrow>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + '</mrow></munder>';
                } else if (parser.subFactor2.fNum == notclosed) {
                    this.tmp = this.tmp + "<mn>sigma[</mn>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "<mn>]</mn>";
                }
                else {
                    this.tmp = this.tmp + "<munderover><mo>&#x2211;</mo><mrow>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "</mrow><mrow>";
                    for (var i = 0; i < parser.subFactor2.pArr.length; i++) {
                        this.Parsering(parser.subFactor2.pArr[i]);
                    }
                    this.tmp = this.tmp + "</mrow></munderover>";
                }
            }
            this.tmp = this.tmp + "</mstyle>";
        }

        // 인자 0개 또는 1개, 보조인자 0개 또는 1개 (0개,1개 경우는 X)
        if (parser.fNum == SQRT) {
            if (parser.subFactor1 == null) {
                if (parser.mainFactor == null) {
                    this.tmp = this.tmp + '<msqrt></msqrt>';
                }
                else {
                    this.tmp = this.tmp + "<msqrt>";
                    for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                        this.Parsering(parser.mainFactor.pArr[i]);
                    }
                    this.tmp = this.tmp + "</msqrt>";
                }
            }
            else if (parser.subFactor1.fNum == notclosed) {
                if (parser.mainFactor == null) {
                    this.tmp = this.tmp + "<mn>sqrt</mn>";
                } else {
                    this.tmp = this.tmp + "<mn>sqrt[</mn>"
                    for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                        this.Parsering(parser.mainFactor.pArr[i]);
                    }
                }
            }
            else {
                if (parser.mainFactor == null) { 
                    this.tmp = this.tmp + "<mn>sqrt[</mn>"
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "]"
                }
                else {
                    if (parser.subFactor1.fNum == empty) {
                        this.tmp = this.tmp + "<msqrt>";
                        for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                            this.Parsering(parser.mainFactor.pArr[i]);
                        }
                        this.tmp = this.tmp + "</msqrt>";
                    } else {
                        this.tmp = this.tmp + "<mroot><mrow>";
                        for (var i = 0; i < parser.mainFactor.pArr.length; i++) {
                            this.Parsering(parser.mainFactor.pArr[i]);
                        }
                        this.tmp = this.tmp + "</mrow><mrow>";
                        for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                            this.Parsering(parser.subFactor1.pArr[i]);
                        }
                        this.tmp = this.tmp + "</mrow></mroot>";
                    }
                }
            }
        }

        // 보조인자 2개 무조건 필요
        if (parser.fNum == OVER) {
            if(parser.subFactor1.fNum == notclosed){
                this.tmp = this.tmp + "<mn>over</mn>";
                return;
            }
            if (parser.subFactor1.fNum == empty) {
                if (parser.subFactor2 == null || parser.subFactor2.fNum == notclosed) {
                    this.tmp = this.tmp + "<mn>over[]</mn>";
                    return;
                }
                if (parser.subFactor2.fNum == empty) {
                    this.tmp = this.tmp + "<mfrac><mrow/><mrow/></mfrac>";
                } else {
                    this.tmp = this.tmp + "<mfrac><mrow/><mrow>";
                    for (var i = 0; i < parser.subFactor2.pArr.length; i++) {
                        this.Parsering(parser.subFactor2.pArr[i]);
                    }
                    this.tmp = this.tmp + "</mrow></mfrac>";
                }
            } else {
                if (parser.subFactor2 == null|| parser.subFactor2.fNum == notclosed) {
                    this.tmp = this.tmp + "<mn>over[";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "]</mn>";
                    return;
                }
                if (parser.subFactor2.fNum == empty) {
                    this.tmp = this.tmp + "<mfrac><mrow>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "</mrow><mrow/></mfrac>";
                } else {
                    this.tmp = this.tmp + "<mfrac><mrow>";
                    for (var i = 0; i < parser.subFactor1.pArr.length; i++) {
                        this.Parsering(parser.subFactor1.pArr[i]);
                    }
                    this.tmp = this.tmp + "</mrow><mrow>";
                    for (var i = 0; i < parser.subFactor2.pArr.length; i++) {
                        this.Parsering(parser.subFactor2.pArr[i]);
                    }
                    this.tmp = this.tmp + "</mrow></mfrac>";
                }
            }
        }

    }
}

