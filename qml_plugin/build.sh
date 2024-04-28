cd ml_plugin
$1/qmake -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && /usr/bin/make qmake_all
make

