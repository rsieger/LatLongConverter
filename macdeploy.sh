#!/bin/bash
echo
echo LatLong Converter

echo - macdeployqt

cd ~/Development/GitHub/LatLongConverter

rm -R '../../Distribution/LatLongConverter/LatLongConverter.app'
cp -R './build-LatLongConverter-Desktop_Qt_5_9_2_clang_64bit-Release/LatLongConverter.app' '../../Distribution/LatLongConverter/LatLongConverter.app'
cp './trunk/Resources/Info.plist' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/Info.plist'

/Developer/Qt/5.9.2/clang_64/bin/macdeployqt '../../Distribution/LatLongConverter/LatLongConverter.app'

echo - code signing

codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/Frameworks/QtCore.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/Frameworks/QtGui.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/Frameworks/QtNetwork.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/Frameworks/QtPrintSupport.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/Frameworks/QtSvg.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/Frameworks/QtWidgets.framework'

codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/bearer/libqcorewlanbearer.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/bearer/libqgenericbearer.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqgif.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqicns.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqico.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqjpeg.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqmacjp2.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqtga.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqtiff.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqwbmp.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/imageformats/libqwebp.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/platforms/libqcocoa.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/printsupport/libcocoaprintersupport.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app/Contents/PlugIns/iconengines/libqsvgicon.dylib'

codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/LatLongConverter/LatLongConverter.app'

echo - mount package

cd ~/Development/Distribution/LatLongConverter
hdiutil attach ~/Development/Distribution/LatLongConverter_OSX.dmg

rm -R '/Volumes/LatLong Converter/LatLongConverter.app'
cp -R LatLongConverter.app '/Volumes/LatLong Converter'

cd '/Volumes/LatLong Converter'
rm -rf .fseventsd
mkdir .fseventsd
touch .fseventsd/no_log
cd ~/Development/Distribution/LatLongConverter

echo - verify package

codesign -d '/Volumes/LatLong Converter/LatLongConverter.app'

echo
hdiutil detach '/Volumes/LatLong Converter'

echo - move application

rm -R /Applications/LatLongConverter.app
cp -R LatLongConverter.app /Applications
cd ~/Development/Distribution

echo - finished
