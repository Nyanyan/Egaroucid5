# Egaroucid5
Strong Othello AI Application based on Egaroucid4, which got 1st place in the world    

**You can use the [application in Japanese](#application_version) or use Python [tkinter version](#tkinter_version)**

**You can [play light version of this AI on the Web](https://www.egaroucid.nyanyan.dev/).**



<a id="application_version"></a>

## アプリの使い方

**The main application is only in Japanese.**

このアプリケーションの仕様について、不便なところや「この機能がほしい！」といった要望を募集しています。作者のTwitter([@Nyanyan_Cube](https://twitter.com/Nyanyan_Cube)または[@takuto_yamana](https://twitter.com/takuto_yamana))またはGitHubのissueやpull requestなどから要望をお送りください。



### 目次

* [フォルダ構成](#folder)
* [対局](#battle)
* [読み手数](#depth)
* [ヒント表示](#hint)
* [評価値/グラフ表示](#value)
* [棋譜解析](#analyze)
* [履歴](#history)
* [入出力](#inout)
* [book](#book)
* [定石](#joseki)



<a id="folder"></a>

### フォルダ構成

アプリの実行前に以下のフォルダとファイルがあるかを確認してください。

```
Egaroucid5.exe
resourcesフォルダ
	book.txt
	joseki.txt
	param.txt
	settings.txt
recordフォルダ
```

```Egaroucid5.exe```をダブルクリックなどで実行するとアプリが起動します。

<a id="battle"></a>

### 対局

最初に手番を```人間先手/人間後手/人間同士/AI同士```から選びます。この項目は対局中に変更ができません。

ヒント表示や評価値表示、読み手数、book誤差は対局中に変更できます。

設定が完了したら```対局開始```ボタンを押すと対局が開始されます。

<a id="depth"></a>

### 読み手数

読み手数は```中盤N手読み/終盤N空読み```のスライドバーで設定できます。

読み手数には、中盤の読み手数と終盤に完全読みする空きマス数の2種類あります。

#### 中盤読み手数

中盤の読み手数はその名前の通り、中盤に何手読むかを決める値です。

前向きな枝刈り(Multi Prob Cut)の確証(どれくらいの確証で読みを早く打ち切るか)は読み手数によって以下のように決まります。確証が高いほど正確な探索になり、100%は前向きな枝刈りをしません。

| 読み手数 | 確証  |
| -------- | ----- |
| [1, 10]  | 100%  |
| [11, 14] | 95.5% |
| [15, 16] | 93.3% |
| [17, 18] | 90.3% |
| [19, 20] | 84.1% |
| [21, 22] | 75.8% |
| [23, 24] | 65.5% |
| [25, 60] | 61.7% |

#### 終盤空読み

終盤に完全読みに入る残り空きマス数を決める値です。

前向きな枝刈り(Multi Prob Cut)の確証(どれくらいの確証で読みを早く打ち切るか)は空きマス数によって以下のように決まります。確証が高いほど正確な探索になり、100%は前向きな枝刈りをしません。

| 読み手数 | 確証  |
| -------- | ----- |
| [1, 18]  | 100%  |
| [19, 20] | 96.4% |
| [21, 22] | 95.5% |
| [23, 24] | 93.3% |
| [25, 26] | 90.3% |
| [27, 28] | 86.4% |
| [29, 30] | 81.6% |
| [31, 32] | 72.6% |
| [33, 60] | 65.5% |

<a id="hint"></a>

### ヒント表示

ヒント表示は局面に存在するすべての合法手に対して行われます。

book登録局面の場合は以下のように評価値と```book```という表示がされます。bookに登録されていない局面はその場で何手か読んで、評価値と読み手数を表示します。ヒント表示の読み手数は```ヒント中盤N手読み/ヒント終盤N空読み```のスライドバーで設定できます。読み手数の内部仕様は上記と同じです。

<a id="value"></a>

### 評価値/グラフ表示

```評価値表示```モードにすると、**AIが手を選択した場合**に画面中央下に評価値が表示され、画面右の評価値グラフが更新されます。AIが手を選択した場合にしか評価値は表示されません。人間が打った手の評価値を表示したいときには```棋譜解析```ボタンを押してください。

#### 評価値

画面中央下に表示される評価値はAI目線の評価値で、双方が最善手を打った場合の最終石差を表します。正の値ならAI有利、負の値ならAI不利と判断しています。

#### 評価値グラフ

評価値グラフは上に行くほど黒優勢、0が互角、下に行くほど白優勢です。双方が最善手を打った場合の予想最終石差を表します。

<a id="analyze"></a>

### 棋譜解析

棋譜解析機能は```中盤N手読み/終盤N空読み```で設定された読み手数において棋譜のすべての手の評価値を算出し、グラフに描画します。

<a id="history"></a>

### 履歴

対局中または対局終了後には、画面中央下に```<```と```>```のボタンが現れます。このボタンを押すことで、手を戻したり進めたりできます。ただし、表示される局面は閲覧専用で、そこから手を進めることはできません

実装が面倒なのでこういう仕様ですが、具体的な要望があったら仕様を変更するかもしれません。

<a id="inout"></a>

### 入出力

画面左下のボタンは入出力に関係するボタンです。



#### 棋譜入力

```棋譜入力```ボタンを押すと、クリップボードにある棋譜を読み込みます。棋譜はF5D6形式、またはf5d6形式に対応しています。

棋譜の読み込みに成功すると```取得成功```、失敗すると```取得失敗```と表示されます。

棋譜を読み込んだら```対局開始```ボタンを押すとその局面から対局できます。



#### 局面入力

```局面入力```ボタンを押すと、クリップボードにある局面を読み込みます。局面は```0```が黒石、```1```が白石、```.```が空きマスとして、盤面を1行で表した64文字に、その局面から打つ手番(```0```か```1```)を加えた65文字に対応しています。例えば、縦取りした局面であれば以下となります。

```
...........................10......100.....1....................0
```

局面の読み込みに成功すると```取得成功```、失敗すると```取得失敗```と表示されます。

局面を読み込んだら```対局開始```ボタンを押すとその局面から対局できます。

局面入力の場合は棋譜データが存在しないので、入力された局面よりも前の局面を履歴から参照することはできません。



#### 対局保存

対局中、または対局終了後に```対局保存```ボタンを押すと、現在の対局を保存できます。

保存先はこのアプリがあるところにある```record```フォルダです。

ファイルはテキストファイルで、名称は以下の通りです。

```
yyyymmdd_hhmmss_N_M_X.txt

yyyy: 年
mm: 月
dd: 日
hh: 時(24時間表記)
mm: 分
ss: 秒
N: AIの中盤読み手数
M: AIの終盤空読み手数
X: モード(0:人間先手 1:人間後手 2:人間同士 3:AI同士)
```

ファイルの中身は以下の形式です。

```
f5d6形式の棋譜 黒の石数 白の石数 黒から見たスコア
```

スコアについては、終局前に保存された棋譜には```?```と表示されます。

保存に成功すると```成功```、失敗すると```失敗```と表示されます。



#### 棋譜コピー

対局中または対局終了後に```棋譜コピー```ボタンを押すと、棋譜がf5d6形式でクリップボードにコピーされます。任意の場所に貼り付けて使ってください。

なお、盤面を```<```や```>```ボタンを使って最後の局面ではない局面に戻していても、コピーされる棋譜は最後の局面までの棋譜です。

これも実装がだるくてこの仕様にしていますが、具体的な要望があれば変更します。



<a id="book"></a>

### book

#### 手の選択におけるbookの使い方

AIが次の一手を選択する際、 その局面での合法手に一つでもbook登録手があった場合には探索は行わず、必ずbookに登録された手を打ちます。

つまり、bookに悪い手ばかり登録すると、AIは悪い手ばかりを打つようになるということです。bookには必ず良い手から順番に登録する必要があります。

bookに登録されている手から、book誤差の成約(この直下で詳しく解説します)のもと、ランダムに手を選択します。



#### book誤差

book誤差は、ある局面において複数の手がbookに登録されているときに有効になります。

ある局面からbookに登録された手のうち、最大の評価値をX、book誤差をYとすると、「X-Y」までの評価値の手をランダムに選びます。

例えばある局面からA、B、Cという3つのbookに登録されている手が選べて、それぞれの評価値が1、-2、-10だったとします。このとき、book誤差が2以下であれば、Aのみが選ばれます。book誤差が3から10までなら、AかBが選ばれます。book誤差が11以上であれば、A、B、Cすべてのうちどれかが選ばれます。



#### bookの手動修正・追加

修正したい局面にセットして、修正したいマスを右クリックします。これでbook修正・追加モードに入ります。

数字キーまたはテンキー(テンキーは動作未確認)とマイナス符号```-```とバックスペースキーを使って修正した評価値を入力してください。入力の確認のため、画面右下に入力した値が出ます。同じマスを再び右クリックすると新しい評価値がbookに登録されます。

入力された文字列が整数に変換できなかった場合は「形式エラー」と表示されます。

誤ってbook修正・追加モードに入ってしまった場合は何も入力せずに(入力欄が空の状態で)同じマスを右クリックすると抜けられます。

bookが変更された場合、アプリの閉じるボタンを押したときにbookを保存するか聞かれます。誤ってbookを登録したときには```保存しない```ボタンを押してください。

誤ってbookを登録して保存してしまった場合は手動で戻すか、resourcesフォルダの```book_backup.txt```を```book.txt```にリネームしてください。



#### bookの自動追加

**外部ツールを使う予定。ツールは後で作る**



<a id="joseki"></a>

### 定石

画面中央上に定石名が表示できる場合は表示されます。

定石データはこちらのものを使っています: http://evaccaneer.livedoor.blog/archives/11101657.html





<a id="tkinter_version"></a>

## Abstract

Egaroucid5 is an Othello AI.

There are former versions:

https://github.com/Nyanyan/Reversi

https://github.com/Nyanyan/Egaroucid

https://github.com/Nyanyan/Egaroucid3

https://github.com/Nyanyan/Egaroucid4



## Requirements

### Languages

* Python3
* C++

### Additional Python libraries

* subprocess
* tkinter

### Additional C++ libraries

None



## How to use with tkinter

First, you have to clone this repository. For example,

```
$ git clone git@github.com:Nyanyan/Egaroucid5.git
```

Then move to the ```src``` directory

```
$ cd Egaroucid5/src
```

Compile ```egaroucid5.cpp```

```
$ g++ egaroucid5.cpp -O3 -march=native -fexcess-precision=fast -funroll-loops -flto -mtune=native -lpthread -Wall -o egaroucid5.out
```

Execute ```main.py```

```
$ python3 main.py
```

Then choose which color AI play. 0 for black, 1 for white

```
AI moves (0: black 1: white): 
```

Press ```Start``` button to play!

