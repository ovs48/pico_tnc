namespace Pico_TNC_GUI
{
    partial class Form1
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.cb_COM = new System.Windows.Forms.ComboBox();
            this.bt_chooseCOM = new System.Windows.Forms.Button();
            this.lb_out = new System.Windows.Forms.ListBox();
            this.bt_test = new System.Windows.Forms.Button();
            this.bt_refreshlist = new System.Windows.Forms.Button();
            this.bt_readCOM = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tb_TXDELAY = new System.Windows.Forms.TextBox();
            this.tb_BeaconDelay = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tb_Unproto = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tb_call = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tb_btext = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tb_alias = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.chk_digipeater = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.RB_GPS_OFF = new System.Windows.Forms.RadioButton();
            this.RB_GPS_GPRMC = new System.Windows.Forms.RadioButton();
            this.RB_GPS_GPGLL = new System.Windows.Forms.RadioButton();
            this.RB_GPS_GPGGA = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.RB_MON_OFF = new System.Windows.Forms.RadioButton();
            this.RB_MON_ME = new System.Windows.Forms.RadioButton();
            this.RB_MON_ALL = new System.Windows.Forms.RadioButton();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.Btn_read = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // cb_COM
            // 
            this.cb_COM.FormattingEnabled = true;
            this.cb_COM.Location = new System.Drawing.Point(13, 13);
            this.cb_COM.Name = "cb_COM";
            this.cb_COM.Size = new System.Drawing.Size(284, 24);
            this.cb_COM.TabIndex = 0;
            // 
            // bt_chooseCOM
            // 
            this.bt_chooseCOM.Location = new System.Drawing.Point(303, 13);
            this.bt_chooseCOM.Name = "bt_chooseCOM";
            this.bt_chooseCOM.Size = new System.Drawing.Size(100, 23);
            this.bt_chooseCOM.TabIndex = 1;
            this.bt_chooseCOM.Text = "Choose Port";
            this.bt_chooseCOM.UseVisualStyleBackColor = true;
            this.bt_chooseCOM.Click += new System.EventHandler(this.button1_Click);
            // 
            // lb_out
            // 
            this.lb_out.FormattingEnabled = true;
            this.lb_out.ItemHeight = 16;
            this.lb_out.Location = new System.Drawing.Point(14, 43);
            this.lb_out.Name = "lb_out";
            this.lb_out.Size = new System.Drawing.Size(283, 356);
            this.lb_out.TabIndex = 2;
            // 
            // bt_test
            // 
            this.bt_test.Location = new System.Drawing.Point(303, 72);
            this.bt_test.Name = "bt_test";
            this.bt_test.Size = new System.Drawing.Size(100, 23);
            this.bt_test.TabIndex = 3;
            this.bt_test.Text = "Flash Mode";
            this.bt_test.UseVisualStyleBackColor = true;
            this.bt_test.Click += new System.EventHandler(this.bt_test_Click);
            // 
            // bt_refreshlist
            // 
            this.bt_refreshlist.Location = new System.Drawing.Point(303, 43);
            this.bt_refreshlist.Name = "bt_refreshlist";
            this.bt_refreshlist.Size = new System.Drawing.Size(100, 23);
            this.bt_refreshlist.TabIndex = 4;
            this.bt_refreshlist.Text = "Refresh COM";
            this.bt_refreshlist.UseVisualStyleBackColor = true;
            this.bt_refreshlist.Click += new System.EventHandler(this.bt_refreshlist_Click);
            // 
            // bt_readCOM
            // 
            this.bt_readCOM.Location = new System.Drawing.Point(303, 104);
            this.bt_readCOM.Name = "bt_readCOM";
            this.bt_readCOM.Size = new System.Drawing.Size(100, 23);
            this.bt_readCOM.TabIndex = 5;
            this.bt_readCOM.Text = "Read DISP";
            this.bt_readCOM.UseVisualStyleBackColor = true;
            this.bt_readCOM.Click += new System.EventHandler(this.bt_readCOM_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(409, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(68, 16);
            this.label1.TabIndex = 6;
            this.label1.Text = "TXDELAY";
            // 
            // tb_TXDELAY
            // 
            this.tb_TXDELAY.Location = new System.Drawing.Point(508, 10);
            this.tb_TXDELAY.Name = "tb_TXDELAY";
            this.tb_TXDELAY.Size = new System.Drawing.Size(280, 22);
            this.tb_TXDELAY.TabIndex = 7;
            this.tb_TXDELAY.Text = "100";
            // 
            // tb_BeaconDelay
            // 
            this.tb_BeaconDelay.Location = new System.Drawing.Point(508, 38);
            this.tb_BeaconDelay.Name = "tb_BeaconDelay";
            this.tb_BeaconDelay.Size = new System.Drawing.Size(280, 22);
            this.tb_BeaconDelay.TabIndex = 9;
            this.tb_BeaconDelay.Text = "1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(409, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(93, 16);
            this.label2.TabIndex = 8;
            this.label2.Text = "Beacon Delay";
            // 
            // tb_Unproto
            // 
            this.tb_Unproto.Location = new System.Drawing.Point(508, 66);
            this.tb_Unproto.Name = "tb_Unproto";
            this.tb_Unproto.Size = new System.Drawing.Size(280, 22);
            this.tb_Unproto.TabIndex = 11;
            this.tb_Unproto.Text = "APRS V WIDE1-1 V WIDE2-2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(409, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(84, 16);
            this.label3.TabIndex = 10;
            this.label3.Text = "Unproto Text";
            // 
            // tb_call
            // 
            this.tb_call.Location = new System.Drawing.Point(508, 94);
            this.tb_call.Name = "tb_call";
            this.tb_call.Size = new System.Drawing.Size(280, 22);
            this.tb_call.TabIndex = 13;
            this.tb_call.Text = "DL1ABC-9";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(409, 97);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(99, 16);
            this.label4.TabIndex = 12;
            this.label4.Text = "Callsign + SSID";
            // 
            // tb_btext
            // 
            this.tb_btext.Location = new System.Drawing.Point(508, 122);
            this.tb_btext.Name = "tb_btext";
            this.tb_btext.Size = new System.Drawing.Size(280, 22);
            this.tb_btext.TabIndex = 15;
            this.tb_btext.Text = "Sample Text";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(409, 125);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(83, 16);
            this.label5.TabIndex = 14;
            this.label5.Text = "Beacon Text";
            // 
            // tb_alias
            // 
            this.tb_alias.Location = new System.Drawing.Point(508, 150);
            this.tb_alias.Name = "tb_alias";
            this.tb_alias.Size = new System.Drawing.Size(280, 22);
            this.tb_alias.TabIndex = 17;
            this.tb_alias.Text = "e.G. WIDE1-1 (useful for digipeater mode)";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(409, 153);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(37, 16);
            this.label6.TabIndex = 16;
            this.label6.Text = "Alias";
            // 
            // chk_digipeater
            // 
            this.chk_digipeater.AutoSize = true;
            this.chk_digipeater.Location = new System.Drawing.Point(650, 178);
            this.chk_digipeater.Name = "chk_digipeater";
            this.chk_digipeater.Size = new System.Drawing.Size(138, 20);
            this.chk_digipeater.TabIndex = 18;
            this.chk_digipeater.Text = "Use as Digipeater";
            this.chk_digipeater.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.RB_GPS_OFF);
            this.groupBox1.Controls.Add(this.RB_GPS_GPRMC);
            this.groupBox1.Controls.Add(this.RB_GPS_GPGLL);
            this.groupBox1.Controls.Add(this.RB_GPS_GPGGA);
            this.groupBox1.Location = new System.Drawing.Point(412, 204);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(376, 76);
            this.groupBox1.TabIndex = 19;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "GPS";
            // 
            // RB_GPS_OFF
            // 
            this.RB_GPS_OFF.AutoSize = true;
            this.RB_GPS_OFF.Location = new System.Drawing.Point(316, 47);
            this.RB_GPS_OFF.Name = "RB_GPS_OFF";
            this.RB_GPS_OFF.Size = new System.Drawing.Size(54, 20);
            this.RB_GPS_OFF.TabIndex = 4;
            this.RB_GPS_OFF.Text = "OFF";
            this.RB_GPS_OFF.UseVisualStyleBackColor = true;
            // 
            // RB_GPS_GPRMC
            // 
            this.RB_GPS_GPRMC.AutoSize = true;
            this.RB_GPS_GPRMC.Location = new System.Drawing.Point(6, 47);
            this.RB_GPS_GPRMC.Name = "RB_GPS_GPRMC";
            this.RB_GPS_GPRMC.Size = new System.Drawing.Size(84, 20);
            this.RB_GPS_GPRMC.TabIndex = 3;
            this.RB_GPS_GPRMC.Text = "$GPRMC";
            this.RB_GPS_GPRMC.UseVisualStyleBackColor = true;
            // 
            // RB_GPS_GPGLL
            // 
            this.RB_GPS_GPGLL.AutoSize = true;
            this.RB_GPS_GPGLL.Location = new System.Drawing.Point(292, 21);
            this.RB_GPS_GPGLL.Name = "RB_GPS_GPGLL";
            this.RB_GPS_GPGLL.Size = new System.Drawing.Size(78, 20);
            this.RB_GPS_GPGLL.TabIndex = 2;
            this.RB_GPS_GPGLL.Text = "$GPGLL";
            this.RB_GPS_GPGLL.UseVisualStyleBackColor = true;
            // 
            // RB_GPS_GPGGA
            // 
            this.RB_GPS_GPGGA.AutoSize = true;
            this.RB_GPS_GPGGA.Checked = true;
            this.RB_GPS_GPGGA.Location = new System.Drawing.Point(6, 21);
            this.RB_GPS_GPGGA.Name = "RB_GPS_GPGGA";
            this.RB_GPS_GPGGA.Size = new System.Drawing.Size(83, 20);
            this.RB_GPS_GPGGA.TabIndex = 1;
            this.RB_GPS_GPGGA.TabStop = true;
            this.RB_GPS_GPGGA.Text = "$GPGGA";
            this.RB_GPS_GPGGA.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.RB_MON_OFF);
            this.groupBox2.Controls.Add(this.RB_MON_ME);
            this.groupBox2.Controls.Add(this.RB_MON_ALL);
            this.groupBox2.Location = new System.Drawing.Point(412, 286);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(376, 58);
            this.groupBox2.TabIndex = 20;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Monitor";
            // 
            // RB_MON_OFF
            // 
            this.RB_MON_OFF.AutoSize = true;
            this.RB_MON_OFF.Location = new System.Drawing.Point(267, 21);
            this.RB_MON_OFF.Name = "RB_MON_OFF";
            this.RB_MON_OFF.Size = new System.Drawing.Size(107, 20);
            this.RB_MON_OFF.TabIndex = 2;
            this.RB_MON_OFF.Text = "Monitoring off";
            this.RB_MON_OFF.UseVisualStyleBackColor = true;
            // 
            // RB_MON_ME
            // 
            this.RB_MON_ME.AutoSize = true;
            this.RB_MON_ME.Location = new System.Drawing.Point(136, 21);
            this.RB_MON_ME.Name = "RB_MON_ME";
            this.RB_MON_ME.Size = new System.Drawing.Size(75, 20);
            this.RB_MON_ME.TabIndex = 1;
            this.RB_MON_ME.Text = "Me only";
            this.RB_MON_ME.UseVisualStyleBackColor = true;
            // 
            // RB_MON_ALL
            // 
            this.RB_MON_ALL.AutoSize = true;
            this.RB_MON_ALL.Checked = true;
            this.RB_MON_ALL.Location = new System.Drawing.Point(6, 21);
            this.RB_MON_ALL.Name = "RB_MON_ALL";
            this.RB_MON_ALL.Size = new System.Drawing.Size(100, 20);
            this.RB_MON_ALL.TabIndex = 0;
            this.RB_MON_ALL.TabStop = true;
            this.RB_MON_ALL.Text = "All Incoming";
            this.RB_MON_ALL.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(412, 379);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(138, 23);
            this.button1.TabIndex = 21;
            this.button1.Text = "Send over Changes";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(650, 379);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(138, 23);
            this.button2.TabIndex = 22;
            this.button2.Text = "Save Permanently";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Btn_read
            // 
            this.Btn_read.Location = new System.Drawing.Point(412, 350);
            this.Btn_read.Name = "Btn_read";
            this.Btn_read.Size = new System.Drawing.Size(376, 23);
            this.Btn_read.TabIndex = 23;
            this.Btn_read.Text = "Read from Modem";
            this.Btn_read.UseVisualStyleBackColor = true;
            this.Btn_read.Click += new System.EventHandler(this.Btn_read_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 411);
            this.Controls.Add(this.Btn_read);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.chk_digipeater);
            this.Controls.Add(this.tb_alias);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.tb_btext);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tb_call);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.tb_Unproto);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tb_BeaconDelay);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tb_TXDELAY);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bt_readCOM);
            this.Controls.Add(this.bt_refreshlist);
            this.Controls.Add(this.bt_test);
            this.Controls.Add(this.lb_out);
            this.Controls.Add(this.bt_chooseCOM);
            this.Controls.Add(this.cb_COM);
            this.Name = "Form1";
            this.Text = "Experimental Pico TNC GUI";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cb_COM;
        private System.Windows.Forms.Button bt_chooseCOM;
        private System.Windows.Forms.ListBox lb_out;
        private System.Windows.Forms.Button bt_test;
        private System.Windows.Forms.Button bt_refreshlist;
        private System.Windows.Forms.Button bt_readCOM;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tb_TXDELAY;
        private System.Windows.Forms.TextBox tb_BeaconDelay;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_Unproto;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tb_call;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tb_btext;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tb_alias;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.CheckBox chk_digipeater;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton RB_MON_OFF;
        private System.Windows.Forms.RadioButton RB_MON_ME;
        private System.Windows.Forms.RadioButton RB_MON_ALL;
        private System.Windows.Forms.RadioButton RB_GPS_OFF;
        private System.Windows.Forms.RadioButton RB_GPS_GPRMC;
        private System.Windows.Forms.RadioButton RB_GPS_GPGLL;
        private System.Windows.Forms.RadioButton RB_GPS_GPGGA;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button Btn_read;
    }
}

