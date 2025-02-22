# Auto Comment Generator
## 概要
Auto Comment Generator は Unreal Engine のブループリントグラフのコメントノードのコメントを自動で生成してくれるプラグインです！

<img width="500" src="https://github.com/user-attachments/assets/4b93b2b5-398a-41d1-9454-7d69a06df612">

プラグイン有効化後に追加したコメントノードの右上に再生ボタンが表示されます。

<img width="500" src="https://github.com/user-attachments/assets/dd58db10-43e5-424f-a01a-4fb74654057e">

この再生ボタンを押してコメントの生成を開始すると再生ボタンが停止ボタンに変わり、コメントの生成を中止する事も出来ます。

<img width="500" src="https://github.com/user-attachments/assets/af9f7e85-a7b8-4594-8b71-875e9db7ab71">

## 設定項目
プラグインを有効化すると「Edit ＞ Editor Preferences ＞ Plugins」に「Auto Comment Generator」というカテゴリーが出現します。ここで Auto Comment Generator に関する設定を行う事が出来ます。
- **_GPT_**
    - **_Open AI API Key_**
        - [Open AI API](https://openai.com/index/openai-api/) で作成可能な API キーです。API キーは外部に流出しないように厳重に管理しましょう。
    - **_GPT Language_**
        - 生成するコメントの言語を設定します。エディタの言語として指定できる言語のみ設定可能です。
    - **_GPT Model Name_**
        - 使用する GPT のモデルを設定します。使用可能なモデルは [Models](https://platform.openai.com/docs/models) から確認できます。「gpt-4」や「gpt-3.5-turbo」等です。あまりにも高性能なモデルを指定すると API 使用料が高くなってしまうので注意しましょう。
- **_Behavior_**
    - **_Ignore Nodes Do Not Have Connected Pins_**
        - コメントノード内にあるノードで入力ピンや出力ピン、実行ピン等が1つも接続されていないものを無視するかどうかを設定します。
    - **_Ignore Comment Nodes_**
        - コメントノード内にあるコメントノードを無視するかどうかを設定します。
    - **_Comment Generation Conditions_**
        - コメントを生成する際の条件を設定します。例えば「answer briefly」と書くと生成されるコメントが簡潔になります。
- **_Appearance_**
    - **_Generate Comment Button Top Padding_**
        - コメント生成ボタンとタイトルバーとの余白を設定します。
    - **_Generate Comment Button Right Padding_**
        - コメント生成ボタンとコメントノードの右端との余白を設定します。
    - **_Generate Comment Button Color_**
        - コメント生成ボタンの色を設定します。
    - **_Generate Comment Button Size_**
        - コメント生成ボタンのサイズを設定します。
# 導入方法
1. [Releases](https://github.com/tsubasamusu/AutoCommentGenerator/releases) から最新のリリースの ZIP ファイルをダウンロードします。
2. ダウンロードした ZIP ファイルをプロジェクトの Plugins フォルダ内で展開します。「{プロジェクト名}/Plugins/AutoCommentGenerator/AutoCommentGenerator.uplugin」となる感じです。
3. プロジェクトのソリューションファイルを再生成し、Visual Studio からビルドします。
4. ビルドに成功したらプロジェクトを開き、「Edit ＞ Plugins ＞ PROJECT ＞ TSUBASAMUSU」にある Auto Comment Generator を有効化してエディタを再起動します。
