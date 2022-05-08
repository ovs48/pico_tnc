using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports; //für COM-Ports

namespace Pico_TNC_GUI
{
    public partial class Form1 : Form
    {
        int current_port = 0;
        string port_name;
        string[] disp = new string[20];
        public Form1()
        {
            InitializeComponent();
            cb_COM.Items.Clear();

            string[] ports = SerialPort.GetPortNames();
            foreach(string port in ports)
            {
                cb_COM.Items.Add(port);
            }
            cb_COM.SelectedIndex = 0;
        }

        void getInfo()
        {
            
        }
        private void button1_Click(object sender, EventArgs e)
        {
            int item = cb_COM.SelectedIndex;
            string choice = cb_COM.Items[item].ToString();
            port_name = choice;
            /*string port = string.Empty;
            for(int i=0; i<choice.Length; i++)
            {
                if (char.IsDigit(choice[i]))
                    port += choice[i];
            }*/
            lb_out.Items.Add("Selected COM Port");
            lb_out.Items.Add(port_name);
            /*current_port = Convert.ToInt32(port);*/
        }

        private void bt_test_Click(object sender, EventArgs e)
        {
            if (port_name != null)
            {
                SerialPort COM = new SerialPort(port_name, 1200);
                try
                {
                    COM.Open();
                }
                catch
                {
                    COM.Close();
                }
            }
            else
                MessageBox.Show("Error: No COM Port or invalid COM Port selected!", "NO COM", MessageBoxButtons.OK, MessageBoxIcon.Error);

        }
        
        void newcommand(SerialPort port)
        {
            port.WriteLine("clear");
            port.Close();
            port.Open();
        }

        private void bt_refreshlist_Click(object sender, EventArgs e)
        {
            cb_COM.Items.Clear();

            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                cb_COM.Items.Add(port);
            }
            cb_COM.SelectedIndex = 0;
        }

        private void bt_readCOM_Click(object sender, EventArgs e)
        {
        bool d=false;
            if (port_name != null)
            {
                string comOut;
                int count = 0;
                SerialPort COM = new SerialPort(port_name, 9600);
                try
                {
                    COM.NewLine = "\r\n";
                    COM.Open();
                    newcommand(COM);
                    COM.WriteLine("disp");
                    lb_out.Items.Clear();
                    while((comOut = COM.ReadLine())!="OK" && !d && comOut!="OK")
                    {
                    if(!comOut.Contains("disp") && !comOut.Equals(""))
                    {
                        //if (count >= 0)
                        //{
                            lb_out.Items.Add(comOut);
                            disp[count] = comOut;
                            count++;
                        //}
                    }
                    if(comOut=="OK") d=true;
                    }
                    COM.Close();
                }
                catch
                {
                    MessageBox.Show("Error while trying to read from COM Port!");
                }
            }
            else
                MessageBox.Show("Error: No COM Port or invalid COM Port selected!", "NO COM", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        string COM_getline(SerialPort COM, int line)
        {
            string output=null;
            for (int i = 0; i <= line; i++)
            {
                output = COM.ReadLine();
            }
            return output;
        }

        int readNumFromStr(string input)
        {
            int num = 0;
            string number=null;
            for(int i = 0; i < input.Length; i++)
            {
                if (Char.IsNumber(input[i])) number += input[i];
            }
            num = Convert.ToInt32(number);
            return num;
        }

        /*string truncString(string input, int start, int stop)
        {
            string output = null;
            char ch;
            int i = 0;
            while(input[start+i]!='\0' && i <= stop)
            {
                ch = input[start + i];
                output += ch;
                i++;
            }
            //output += '\0';
            lb_out.Items.Add(output);
            return output;
        }*/

        private void Btn_read_Click(object sender, EventArgs e)
        {
            if (port_name != null)
            {
                string temp;
                try
                {
                    bt_readCOM_Click(null, null);
                    int i = 1;

                    //TX Delay lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    tb_TXDELAY.Text = readNumFromStr(temp).ToString();
                    
                    //GPS lesen
                    temp = lb_out.Items[i].ToString(); i+=2;
                    temp = temp.Replace("GPS ", "");
                    if (temp.Equals("$GPGGA")) RB_GPS_GPGGA.Checked = true;
                    else if (temp.Equals("$GPRMC")) RB_GPS_GPRMC.Checked = true;
                    else if (temp.Equals("$GPGLL")) RB_GPS_GPGLL.Checked = true;
                    else RB_GPS_OFF.Checked = true;

                    //Monitor lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    temp = temp.Replace("MONitor ", "");
                    if (temp.Equals("ALL")) RB_MON_ALL.Checked = true;
                    else if (temp.Equals("ME")) RB_MON_ME.Checked = true;
                    else RB_MON_OFF.Checked = true;

                    //Digipeater lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    temp = temp.Replace("DIGIpeater ", "");
                    if (temp.Equals("ON")) chk_digipeater.Checked = true;
                    else chk_digipeater.Checked = false;

                    //Beacon Delay lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    tb_BeaconDelay.Text = readNumFromStr(temp).ToString();

                    //Unproto lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    tb_Unproto.Text = temp.Replace("UNPROTO ", "");

                    //MYCALL lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    tb_call.Text = temp.Replace("MYCALL ", "");

                    //Alias lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    tb_alias.Text = temp.Replace("MYALIAS ", "");
                    
                    //btext lesen
                    temp = lb_out.Items[i].ToString(); i++;
                    tb_btext.Text = temp.Replace("BTEXT ", "");

                    lb_out.Items.Add("Read OK");
                }
                catch
                {
                    MessageBox.Show("Error while trying to read!");
                }
            }
            else
                MessageBox.Show("Error: No COM Port or invalid COM Port selected!", "NO COM", MessageBoxButtons.OK, MessageBoxIcon.Error);

        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            string[] param = new string[9];
            int i_param;
            try
            {
                param[0] = string.Format("TXDELAY {0}", tb_TXDELAY.Text);   //TXDELAY

                i_param = Convert.ToInt32(tb_BeaconDelay.Text);             //Beacon Delay
                if (i_param == 0) param[1] = "BEACON OFF";
                else param[1] = String.Format("BEACON EVERY {0}", i_param);

                param[2] = String.Format("UNPROTO {0}", tb_Unproto.Text);

                param[3] = String.Format("MYCALL {0}", tb_call.Text);

                param[4] = String.Format("BTEXT {0}", tb_btext.Text);

                param[5] = String.Format("MYALIAS {0}", tb_alias.Text);
                //param[5] = null;

                if (chk_digipeater.Checked == true) param[6] = "DIGI ON";
                else param[6] = "DIGI OFF";

                if (RB_GPS_GPGGA.Checked == true) param[7] = "GPS $GPGGA";
                else if (RB_GPS_GPGLL.Checked == true) param[7] = "GPS $GPGLL";
                else if (RB_GPS_GPRMC.Checked == true) param[7] = "GPS $GPRMC";
                else param[7] = "GPS OFF";

                if (RB_MON_ALL.Checked == true) param[8] = "MON ALL";
                else if (RB_MON_ME.Checked == true) param[8] = "MON ME";
                else param[8] = "MON OFF";
            }
            catch
            {
                MessageBox.Show("Please check Parameters!");
                return;
            }

            try
            {
                SerialPort COM = new SerialPort(port_name, 9600);
                COM.NewLine = "\r\n";
                COM.Open();
                for (int i = 0; i < 9; i++)
                    COM.WriteLine(param[i]);
                COM.Close();
                lb_out.Items.Add("Write OK!");
            }
            catch
            {
                MessageBox.Show("Error Writing Data!");
                return;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                SerialPort COM = new SerialPort(port_name, 9600);
                COM.NewLine = "\r\n";
                COM.Open();
                COM.WriteLine("PERM");
                COM.Close();
                lb_out.Items.Add("Save OK!");
            }
            catch
            {
                MessageBox.Show("Error Writing Data!");
                return;
            }
        }
    }
}
