#!/bin/bash

sudo apt-get install mono-complete git
#Download here
#mkdir gui-linux
#cd gui-linux
#wget https://raw.githubusercontent.com/ovs48/pico_tnc/master/Pico%20TNC%20GUI/Pico%20TNC%20GUI/Form1.cs
#wget https://raw.githubusercontent.com/ovs48/pico_tnc/master/Pico%20TNC%20GUI/Pico%20TNC%20GUI/Program.cs
#wget https://raw.githubusercontent.com/ovs48/pico_tnc/master/Pico%20TNC%20GUI/Pico%20TNC%20GUI/Form1.Designer.cs
#wget https://raw.githubusercontent.com/ovs48/pico_tnc/master/Pico%20TNC%20GUI/Pico%20TNC%20GUI/App.config
#wget https://raw.githubusercontent.com/ovs48/pico_tnc/master/Pico%20TNC%20GUI/Pico%20TNC%20GUI/Form1.resx

cd "Pico TNC GUI/Pico TNC GUI"

#compile in mcs
mcs *.cs -r:System.Windows.Forms.dll,System.Data.dll,System.Drawing.dll -o ../../picotnc-gui.exe

#build standalone
cd ../..
mkbundle -o picotnc-gui --simple picotnc-gui.exe --no-machine-config --no-config
echo "Please run ./picotnc-gui as sudo!"
