# CURLをC++から使ってみよう
## 概要

この勉強会でやってみた内容の復習

https://supporterzcolab.com/event/595/

勉強会のサンプルではcharだったけど、string使うようにしてる。

## 実行方法

`execute.sh` を叩くだけ

## 叩いているところ

サポーターズColabのAPI

https://supporterzcolab.com/about/api/

そしてなんか、たまにブラウザとか、terminalからCURLしないとバグる。なぜ？

## その他に参考にしたところ

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
