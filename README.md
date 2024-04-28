# TestImagesCompression

## 1.  Проект
Проект складається з трьох частин:
    - compression -> бібліотека для стискання зображеннь
        2 методи:
          - compress   -> BMP -> branch
          - decompress -> branch -> BMP
    - qml_plugin - бібліотека що включає усі QML форми
    - ui - реалізація UI через QtWidgets та бізнес-логіка

## 2. Параметри системи
Параметри ОС
   - Distributor ID:	Ubuntu
   - Description:	Ubuntu 22.04.4 LTS
   - Release:	22.04
   - Codename:	jammy
Параметри Qt
   - QMake version 3.1
   - Using Qt version 5.15.3 in /usr/lib/x86_64-linux-gnu

## 3. Build проекту
    ДО початку збірки проекту необхіднвстановити executable на усі наступні файли:
        /.../TestImages/build_and_start.sh
        /.../TestImages/cmake_build.sh
        /.../TestImages/qml_build.sh
        /.../TestImages/start.sh
        /.../TestImages/qml_plugin/build.sh
        /.../TestImages/bin/compress_image_ui
        /.../TestImages/bin/start.sh
    Скрипт для збірки ...TestImages/
    ./build_and_start.sh
Перед білдом необхідно вставити в build_and_start.sh три шляхи:
    QT_BIN_PATH='/home/.../Qt/6.6.1/gcc_64/bin/' - шлях до qmake
    CMAKE_BIN_PATH="/home/.../Qt/6.6.1/gcc_64/lib/cmake/"  - шлях до cmake
    TARGET_LIBRARY="/home/.../TestPacketBook/IMAGES/" -  - шлях до папки з малюнками
Папка для згенерованих файллв - build
У випадку запуску build_and_start.sh використовується qmake, а з будь-яким параметром через cmake
    Example:
        build_and_start.sh -> qmake
        build_and_start.sh AAA -> cmake
## 4. Запуск app
    ./start.sh
    Або перед запуском вкадати шлях до compression.so:
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:LIB_PATH
        LIB_PATH - шлях до бібліотеки
        чи буть-який інших метод для визначення шляху для бібіліотеки
    ./compress_image_ui
    compress_image_ui --help -щоб отримати інформацію
    compress_image_ui -t PATH_TO_IMAGES
Зібраний проект під лінукс знаходиться у bin

## 5. Додаткові особливості
Викоритання project.pro можна використати для QtCreator але не згенрувати оскільки install не налаштований. Аналогічно і CMakeLists.txt.
Всі налаштування зроблені build_and_start.sh
збірка бібліотеки qlm_plugin через qt_add_qml_module видає дивну помилку:
    - "Qt plugin metadata not valid" :(
тож для отримання qlm_plugin через cmake реально використовується qmake

PS: соррі за UI кольори. UI-designer з мене не дуже 
