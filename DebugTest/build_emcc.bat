@echo off
setlocal enabledelayedexpansion

rem バッチファイルのある場所をカレントディレクトリにする
cd /d "%~dp0"

rem emsdkの初期化（環境変数が未設定の場合のみ）
if not defined EMSDK (
    where emsdk >nul 2>&1
    if %errorlevel% == 0 (
        call emsdk_env.bat
    ) else (
        set EMSDK_DIR=%~dp0emsdk
        if not exist "!EMSDK_DIR!" (
            echo [情報] emsdkが見つかりません。
            echo プロジェクト内にダウンロードしますか？（数GBの空き容量が必要です）
            set /p ANSWER="ダウンロードしますか？ [y/N]: "
            if /i "!ANSWER!" neq "y" (
                echo [情報] ダウンロードをキャンセルしました。
                pause
                exit /b 0
            )

            echo [情報] emsdkをダウンロードしています...
            git clone https://github.com/emscripten-core/emsdk.git "!EMSDK_DIR!"
            if !errorlevel! neq 0 (
                echo [エラー] emsdkのダウンロードに失敗しました。
                echo gitがインストールされているか、ネットワーク接続を確認してください。
                pause
                exit /b 1
            )

            echo [情報] emsdkをインストールしています...
            cd /d "!EMSDK_DIR!"
            call emsdk install latest
            if !errorlevel! neq 0 (
                echo [エラー] emsdkのインストールに失敗しました。
                pause
                exit /b 1
            )

            echo [情報] emsdkを有効化しています...
            call emsdk activate latest
            if !errorlevel! neq 0 (
                echo [エラー] emsdkの有効化に失敗しました。
                pause
                exit /b 1
            )

            cd /d "%~dp0"
        )

        call "!EMSDK_DIR!\emsdk_env.bat"
        if !errorlevel! neq 0 (
            echo [エラー] emsdk_env.batの実行に失敗しました。
            pause
            exit /b 1
        )
    )
)

rem サードパーティライブラリのフォルダ内の.jsファイルを全て--pre-jsに追加
set PRE_JS_ARGS=
for /r ./include/DebugPrint/third_party %%f in (*.js) do (
    set PRE_JS_ARGS=!PRE_JS_ARGS! --pre-js "%%f"
)

rem 出力先フォルダが存在しない場合は作成する
if not exist "./docs" mkdir "./docs"

echo [情報] コンパイルを開始します...
emcc main.cpp -std=c++20 -I./include ^
    -DDEBUG_PRINT_IMPLEMENTATION ^
    -DDEBUG_PRINT_ENABLED ^
    -O2 ^
    -sASYNCIFY ^
    -sASYNCIFY_STACK_SIZE=131072 ^
    --shell-file ./shell.html ^
    %PRE_JS_ARGS% ^
    -o ./docs/index.html
if %errorlevel% neq 0 (
    echo [エラー] コンパイルに失敗しました。
    pause
    exit /b 1
)

echo [情報] コンパイルが完了しました。
pause