# 概要

[【サポーターズCoLab勉強会】C++とcurlでHTTP通信処理作ってみた](https://supporterzcolab.com/event/595/) こちらの勉強会に参加してみたので、自分でも書いてみた。

サンプルとして[サポーターズColabAPI](https://supporterzcolab.com/about/api/)を叩いてみて（ConnpassのAPI叩いた経験がある人はわかるけど、アレとだいたい一緒）、取得できたJSONレスポンスを、picojsonでパースしてみて値を表示しようとした。

あととりあえず動かしてみた、程度なのでクオリティは低め。（最近Qiita記事の質がどうこうの話がある中、）すみません。

前提はMacでやっています。

# 環境

* OS: MacOS 10.14（18A391）
* GCC version： Homebrew GCC 8.2.0) 8.2.0
* curl version: curl 7.54.0

# 前準備

Curlをインストールする方法は多数があるが、今回は手っ取り早く動かすためにbrewでインストールする。

```shell
brew install curl
```

# アクセス部分

こんな感じになった。

```cpp
size_t onReceive(char* ptr, size_t size, size_t nmemb, std::string* stream) {
    const size_t sizes = size * nmemb;
    stream->append(ptr, sizes);
    return sizes;
}

std::string get_responce() {
    CURL *curl = curl_easy_init();
    if(curl == nullptr) {
        std::cerr << "curl init error" << std::endl;
        curl_easy_cleanup(curl);
        exit(1);
    }

    std::string ret;
    // setup
    curl_easy_setopt(curl, CURLOPT_URL, "https://supporterzcolab.com/api/v1/event/?event_id=586");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);

    CURLcode res = curl_easy_perform(curl);
    // using sleep time, Succeed
    sleep(5);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }
    if(ret.length() == 0) {
        std::cerr << "respnceData is null, maybe timeout" << std::endl;
        std::cerr << "responceData: " << ret << std::endl;
        exit(1);
    }
    return ret;
}
```

本当はURLとか引数に渡せばいいような気もするけど、とりあえずこれで。（私自身は普段Pythonを書いているので、とりあえず設定はこの辺すべてDictで渡したくもなるが、まあとりあえずで…）

だいたいやることの流れとしては

* `#include<curl/curl.h>` をinclude(上記では省略してしまっているけど)
* `curl_easy_init()` でインスタンスを作る
* `curl_easy_setopt（）` でひたすら設定を書く。以下のような認識をしている。
    * CURLOPT_URL: 接続先URL
    * CURLOPT_TIMEOUT: タイムアウト秒数
    * CURLOPT_SSL_VERIFYPEER: SSL検証。ONでも動きそうだが、一旦OFFに。
    * CURLOPT_WRITEFUNCTION： CALLBACKする関数名
    * CURLOPT_WRITEDATA： データ書き込み先
* CALLBACKする関数を書いておく
* `curl_easy_perform(curl)` を実行する
* エラーハンドリングと結果の加工

みたいな感じ。

あとどうもタイムアウトを入れないとうまくいかない場合が多かったので入れているけれども、どうもネット上に転がっているサンプルを見ると入れなくて済んでいるので、多分このAPI依存の問題っぽいぞ、みたいなのは感じた。

# picojson でのパース

こうなった。

```cpp
    std::string responce = get_responce();

    picojson::value json_val;
    // error check
    std::string err_check = picojson::parse(json_val, responce);
    if (!err_check.empty()) {
        std::cerr << "json parse error: " << err_check << std::endl;
        return 1;
    }

    picojson::object& obj = json_val.get<picojson::object>();
    picojson::array& events = obj["events"].get<picojson::array>();
    std::string title, started_at;
    for(const auto& event : events) {
        title = event.get<picojson::object>().at("title").get<std::string>();
        started_at = event.get<picojson::object>().at("started_at").get<std::string>();
    }
    std::cout << title << std::endl;
    std::cout << started_at << std::endl;
```

おお、確かにアクセスちょっと楽に書けるかも。昔はゴリゴリ書いているなにかを読んでいて頭を壊しましたが、これはスマートですね。

今回、Arrayであるのにもかかわらず配列とかSUMをとるような計算をしていないのは、今回ループ処理で入るものが1つだとわかっているので手抜いているためです。

こっちもやることとしては

* `picojson::value` インスタンス生成
* `picojson::parse(json_val, responce)` でJSONデータを読み込む
* あとはよしなに、arrayなりなんなりに突っ込んでとる
* arrayはfor-eachループで処理してやる
* 最後に取り出す時は型を指定してやる感じ

みたいな流れ。ちなみにjsonはめっちゃ簡略化してこんな感じのやつが取得できてる。

```json
{
  "results_returned": 1,
  "events": [
    {
      ~~(中略)~~
      "started_at": "2018-11-22T19:30:00+09:00",
      "hash_tag": "spzcolab",
      "title": "【サポーターズCoLab勉強会】Unity ライブゲーム制作 〜音ゲー編〜",
      "event_id": 562,
      ~~(中略)~~
    }
  ],
  "results_start": 1,
  "results_available": 1
}
```

# 完成したコード

全部貼ると長いので、ここ参照。

https://github.com/yumechi/selfstudy/blob/master/other/CPP/using_uCRL/test_curl.cpp

# 実行する

`-lcurl` を含めてコンパイルする必要があるが、めんどくさいのでShellを用意する。

```shell
#!/bin/bash

FILENAME="test_curl"
LIBFILE="picojson.h"

if [ ! -e ${LIBFILE} ]; then
    echo "download ${LIBFILE}"
    curl https://raw.githubusercontent.com/kazuho/picojson/master/picojson.h > ${LIBFILE}
fi

g++ ${FILENAME}.cpp -std=c++11 -o ${FILENAME} -lcurl
./${FILENAME} 
```

`chmod u+x execute.sh` してから実行してみる。

```shell
[11/12 03:42:25] $ ./execute.sh
【サポーターズ】CoLabのみんなで技術書デビュー！しちゃおう！
2018-11-29T19:45:00+09:00
```

想定した通り、JSONファイルから勉強会のタイトルと開始時間が取得できた。

# 解決できていないこと

このAPIの問題のようだが、たまにレスポンスが取得ができないことがあり、手動で `curl "https://supporterzcolab.com/api/v1/event/?event_id=586"` してから再度実行するとうまく動作する。キャッシュとかの問題なのかな…。

# 感想

あ、普段はPythonでrequests使ってアクセスしてimport jsonして一瞬でやっているので、C++出会えてやってみると結構大変でした。たくさん調べる必要があったし・・・。


結構原始的にゴリゴリ書いていったわけだけれども、CURLの設定道だったかなとか、picojsonどう使うんだうおーってなりながらも、たくさんの先人のおかげでなんとか動くものができましたとさ。良かったよかった。

もう少し抽象的に書けると良さそうなので、あまり続きを書く気はないですが、続きがあればリクエスト部分の抽象化、POST等別メソッドを使ってみる、JSONパースももう少し抽象化して取得できるようにする、等を意識して書きたいなと思います。

ともあれ、リクエストができる、JSONが扱えるということはC++でもサーバー側書けたりしてね。C++はプログラミングコンテストでしか普段書かないので、ちょっと新鮮でした。（あるいはWindowsApplicationにちょっとした機能として組み込むとかもできるのかな？）

# 参考にした記事

羅列ですみません。

* CURL関連
    * C/C++のlibcurl利用サンプル
        * https://qiita.com/alaif-net@github/items/e4432c374985dc38e038
    * curl_setopt
        * http://php.net/manual/ja/function.curl-setopt.php
* C++関連
    * kazuho/picojson
        * https://github.com/kazuho/picojson
        * C++ のJSONパースライブラリ
    * PicoJSONでarray型の読み込み
        * https://ja.stackoverflow.com/questions/14240/picojson%E3%81%A7array%E5%9E%8B%E3%81%AE%E8%AA%AD%E3%81%BF%E8%BE%BC%E3%81%BF
    * 【C++】unistd.hのsleep()で1秒未満の値を設定する
        * https://qiita.com/hashimotoryoh/items/609ed5bf4dd4cd90178a
        * なんか調子悪いのでタイムアウトを入れている
        * 結局このアクセス先のAPI依存っぽい？
    * C++のJSONパーサ『picojson』をためしてみました。
        * https://qiita.com/ayase/items/58c9e63e96857d83ebd9
    * C++でunicode escape sequenceをutf8にする
        * https://qiita.com/kumagi/items/f45742d9a312e27769b1
        * なんかうまく動かなかった
    * 俺でもわかるPicojsonの使い方
        * http://akira206.hatenablog.com/entry/2016/06/26/204801
    * picojsonのネスト構造取得サンプル
        * https://blog.systemjp.net/entry/207/06/08/191331
* 実行ファイル関連
    * シェルスクリプトでファイル存在確認
        * http://d.hatena.ne.jp/fantasista_ik/20090825/1251183315
    * エスケープされた日本語文字列をデコードしたい
        * https://ja.stackoverflow.com/questions/217/%E3%82%A8%E3%82%B9%E3%82%B1%E3%83%BC%E3%83%97%E3%81%95%E3%82%8C%E3%81%9F%E6%97%A5%E6%9C%AC%E8%AA%9E%E6%96%87%E5%AD%97%E5%88%97%E3%82%92%E3%83%87%E3%82%B3%E3%83%BC%E3%83%89%E3%81%97%E3%81%9F%E3%81%84
        * Python2を使うような感じなのでもう少しうまくやりたい
* APIテスト先
    * サポーターズCoLab API リファレンス
        * https://supporterzcolab.com/about/api/

