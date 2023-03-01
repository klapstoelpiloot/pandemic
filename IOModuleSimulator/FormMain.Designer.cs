namespace IOModuleSimulator
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.buttonsgroup = new System.Windows.Forms.GroupBox();
            this.buttoncancel = new System.Windows.Forms.Button();
            this.buttonok = new System.Windows.Forms.Button();
            this.buttonright = new System.Windows.Forms.Button();
            this.buttonleft = new System.Windows.Forms.Button();
            this.bridgegroup = new System.Windows.Forms.GroupBox();
            this.buttonbridgecleared = new System.Windows.Forms.Button();
            this.buttonremovepuck = new System.Windows.Forms.Button();
            this.buttonbridgeblocked = new System.Windows.Forms.Button();
            this.buttonstartslide = new System.Windows.Forms.Button();
            this.buttonbridgefront = new System.Windows.Forms.Button();
            this.buttonbridgeback = new System.Windows.Forms.Button();
            this.connectiongroup = new System.Windows.Forms.GroupBox();
            this.connectbutton = new System.Windows.Forms.Button();
            this.connectpipe = new System.Windows.Forms.TextBox();
            this.connectpipelabel = new System.Windows.Forms.Label();
            this.connectaddress = new System.Windows.Forms.TextBox();
            this.connectaddresslabel = new System.Windows.Forms.Label();
            this.gatesgroup = new System.Windows.Forms.GroupBox();
            this.buttonscoregate4 = new System.Windows.Forms.Button();
            this.buttongate4front = new System.Windows.Forms.Button();
            this.buttongate4back = new System.Windows.Forms.Button();
            this.buttonscoregate3 = new System.Windows.Forms.Button();
            this.buttongate3front = new System.Windows.Forms.Button();
            this.buttongate3back = new System.Windows.Forms.Button();
            this.buttonscoregate2 = new System.Windows.Forms.Button();
            this.buttongate2front = new System.Windows.Forms.Button();
            this.buttongate2back = new System.Windows.Forms.Button();
            this.buttonscoregate1 = new System.Windows.Forms.Button();
            this.buttongate1front = new System.Windows.Forms.Button();
            this.buttongate1back = new System.Windows.Forms.Button();
            this.animateSensorTimer = new System.Windows.Forms.Timer(this.components);
            this.focusbox = new System.Windows.Forms.PictureBox();
            this.buttonsgroup.SuspendLayout();
            this.bridgegroup.SuspendLayout();
            this.connectiongroup.SuspendLayout();
            this.gatesgroup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.focusbox)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonsgroup
            // 
            this.buttonsgroup.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonsgroup.Controls.Add(this.buttoncancel);
            this.buttonsgroup.Controls.Add(this.buttonok);
            this.buttonsgroup.Controls.Add(this.buttonright);
            this.buttonsgroup.Controls.Add(this.buttonleft);
            this.buttonsgroup.Enabled = false;
            this.buttonsgroup.Location = new System.Drawing.Point(12, 353);
            this.buttonsgroup.Name = "buttonsgroup";
            this.buttonsgroup.Size = new System.Drawing.Size(581, 122);
            this.buttonsgroup.TabIndex = 0;
            this.buttonsgroup.TabStop = false;
            this.buttonsgroup.Text = " Buttons ";
            // 
            // buttoncancel
            // 
            this.buttoncancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttoncancel.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("buttoncancel.BackgroundImage")));
            this.buttoncancel.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttoncancel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttoncancel.Location = new System.Drawing.Point(477, 30);
            this.buttoncancel.Name = "buttoncancel";
            this.buttoncancel.Size = new System.Drawing.Size(74, 74);
            this.buttoncancel.TabIndex = 3;
            this.buttoncancel.TabStop = false;
            this.buttoncancel.Text = "L";
            this.buttoncancel.UseVisualStyleBackColor = true;
            this.buttoncancel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttoncancel_MouseDown);
            this.buttoncancel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttoncancel_MouseUp);
            // 
            // buttonok
            // 
            this.buttonok.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonok.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("buttonok.BackgroundImage")));
            this.buttonok.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttonok.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonok.Location = new System.Drawing.Point(358, 30);
            this.buttonok.Name = "buttonok";
            this.buttonok.Size = new System.Drawing.Size(74, 74);
            this.buttonok.TabIndex = 2;
            this.buttonok.TabStop = false;
            this.buttonok.Text = "K";
            this.buttonok.UseVisualStyleBackColor = true;
            this.buttonok.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonok_MouseDown);
            this.buttonok.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonok_MouseUp);
            // 
            // buttonright
            // 
            this.buttonright.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("buttonright.BackgroundImage")));
            this.buttonright.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttonright.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonright.Location = new System.Drawing.Point(149, 30);
            this.buttonright.Name = "buttonright";
            this.buttonright.Size = new System.Drawing.Size(74, 74);
            this.buttonright.TabIndex = 1;
            this.buttonright.TabStop = false;
            this.buttonright.Text = "S";
            this.buttonright.UseVisualStyleBackColor = true;
            this.buttonright.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonright_MouseDown);
            this.buttonright.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonright_MouseUp);
            // 
            // buttonleft
            // 
            this.buttonleft.BackgroundImage = global::IOModuleSimulator.Properties.Resources.button_gray;
            this.buttonleft.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttonleft.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonleft.Location = new System.Drawing.Point(30, 30);
            this.buttonleft.Name = "buttonleft";
            this.buttonleft.Size = new System.Drawing.Size(74, 74);
            this.buttonleft.TabIndex = 0;
            this.buttonleft.TabStop = false;
            this.buttonleft.Text = "A";
            this.buttonleft.UseVisualStyleBackColor = true;
            this.buttonleft.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonleft_MouseDown);
            this.buttonleft.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonleft_MouseUp);
            // 
            // bridgegroup
            // 
            this.bridgegroup.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.bridgegroup.Controls.Add(this.buttonbridgecleared);
            this.bridgegroup.Controls.Add(this.buttonremovepuck);
            this.bridgegroup.Controls.Add(this.buttonbridgeblocked);
            this.bridgegroup.Controls.Add(this.buttonstartslide);
            this.bridgegroup.Controls.Add(this.buttonbridgefront);
            this.bridgegroup.Controls.Add(this.buttonbridgeback);
            this.bridgegroup.Enabled = false;
            this.bridgegroup.Location = new System.Drawing.Point(12, 229);
            this.bridgegroup.Name = "bridgegroup";
            this.bridgegroup.Size = new System.Drawing.Size(581, 118);
            this.bridgegroup.TabIndex = 1;
            this.bridgegroup.TabStop = false;
            this.bridgegroup.Text = " Bridge ";
            // 
            // buttonbridgecleared
            // 
            this.buttonbridgecleared.Location = new System.Drawing.Point(461, 30);
            this.buttonbridgecleared.Name = "buttonbridgecleared";
            this.buttonbridgecleared.Size = new System.Drawing.Size(90, 66);
            this.buttonbridgecleared.TabIndex = 9;
            this.buttonbridgecleared.TabStop = false;
            this.buttonbridgecleared.Text = "Sensor\r\nCleared";
            this.buttonbridgecleared.UseVisualStyleBackColor = true;
            this.buttonbridgecleared.Click += new System.EventHandler(this.buttonbridgecleared_Click);
            this.buttonbridgecleared.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonremovepuck
            // 
            this.buttonremovepuck.Location = new System.Drawing.Point(215, 30);
            this.buttonremovepuck.Name = "buttonremovepuck";
            this.buttonremovepuck.Size = new System.Drawing.Size(90, 66);
            this.buttonremovepuck.TabIndex = 8;
            this.buttonremovepuck.TabStop = false;
            this.buttonremovepuck.Text = "Remove\r\nPuck";
            this.buttonremovepuck.UseVisualStyleBackColor = true;
            this.buttonremovepuck.Click += new System.EventHandler(this.buttonremovepuck_Click);
            this.buttonremovepuck.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonbridgeblocked
            // 
            this.buttonbridgeblocked.Location = new System.Drawing.Point(338, 30);
            this.buttonbridgeblocked.Name = "buttonbridgeblocked";
            this.buttonbridgeblocked.Size = new System.Drawing.Size(90, 66);
            this.buttonbridgeblocked.TabIndex = 7;
            this.buttonbridgeblocked.TabStop = false;
            this.buttonbridgeblocked.Text = "Sensor\r\nBlocked";
            this.buttonbridgeblocked.UseVisualStyleBackColor = true;
            this.buttonbridgeblocked.Click += new System.EventHandler(this.buttonbridgeblocked_Click);
            this.buttonbridgeblocked.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonstartslide
            // 
            this.buttonstartslide.Location = new System.Drawing.Point(92, 30);
            this.buttonstartslide.Name = "buttonstartslide";
            this.buttonstartslide.Size = new System.Drawing.Size(90, 66);
            this.buttonstartslide.TabIndex = 6;
            this.buttonstartslide.TabStop = false;
            this.buttonstartslide.Text = "Start\r\nSlide";
            this.buttonstartslide.UseVisualStyleBackColor = true;
            this.buttonstartslide.Click += new System.EventHandler(this.buttonstartslide_Click);
            this.buttonstartslide.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonbridgefront
            // 
            this.buttonbridgefront.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttonbridgefront.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttonbridgefront.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonbridgefront.Location = new System.Drawing.Point(30, 66);
            this.buttonbridgefront.Name = "buttonbridgefront";
            this.buttonbridgefront.Size = new System.Drawing.Size(30, 30);
            this.buttonbridgefront.TabIndex = 5;
            this.buttonbridgefront.TabStop = false;
            this.buttonbridgefront.UseVisualStyleBackColor = true;
            this.buttonbridgefront.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttonbridgefront.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonbridgeback
            // 
            this.buttonbridgeback.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_on;
            this.buttonbridgeback.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttonbridgeback.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonbridgeback.Location = new System.Drawing.Point(30, 30);
            this.buttonbridgeback.Name = "buttonbridgeback";
            this.buttonbridgeback.Size = new System.Drawing.Size(30, 30);
            this.buttonbridgeback.TabIndex = 4;
            this.buttonbridgeback.TabStop = false;
            this.buttonbridgeback.UseVisualStyleBackColor = true;
            this.buttonbridgeback.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttonbridgeback.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // connectiongroup
            // 
            this.connectiongroup.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.connectiongroup.Controls.Add(this.connectbutton);
            this.connectiongroup.Controls.Add(this.connectpipe);
            this.connectiongroup.Controls.Add(this.connectpipelabel);
            this.connectiongroup.Controls.Add(this.connectaddress);
            this.connectiongroup.Controls.Add(this.connectaddresslabel);
            this.connectiongroup.Location = new System.Drawing.Point(12, 12);
            this.connectiongroup.Name = "connectiongroup";
            this.connectiongroup.Size = new System.Drawing.Size(581, 85);
            this.connectiongroup.TabIndex = 1;
            this.connectiongroup.TabStop = false;
            this.connectiongroup.Text = " Connection ";
            // 
            // connectbutton
            // 
            this.connectbutton.Location = new System.Drawing.Point(459, 33);
            this.connectbutton.Name = "connectbutton";
            this.connectbutton.Size = new System.Drawing.Size(92, 28);
            this.connectbutton.TabIndex = 4;
            this.connectbutton.TabStop = false;
            this.connectbutton.Text = "Connect";
            this.connectbutton.UseVisualStyleBackColor = true;
            this.connectbutton.Click += new System.EventHandler(this.connectbutton_Click);
            this.connectbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // connectpipe
            // 
            this.connectpipe.Location = new System.Drawing.Point(288, 35);
            this.connectpipe.Name = "connectpipe";
            this.connectpipe.Size = new System.Drawing.Size(152, 23);
            this.connectpipe.TabIndex = 3;
            this.connectpipe.Text = "pandemic_iomodule";
            // 
            // connectpipelabel
            // 
            this.connectpipelabel.AutoSize = true;
            this.connectpipelabel.Location = new System.Drawing.Point(216, 38);
            this.connectpipelabel.Name = "connectpipelabel";
            this.connectpipelabel.Size = new System.Drawing.Size(66, 15);
            this.connectpipelabel.TabIndex = 2;
            this.connectpipelabel.Text = "Pipe name:";
            // 
            // connectaddress
            // 
            this.connectaddress.Location = new System.Drawing.Point(92, 35);
            this.connectaddress.Name = "connectaddress";
            this.connectaddress.Size = new System.Drawing.Size(118, 23);
            this.connectaddress.TabIndex = 1;
            this.connectaddress.Text = ".";
            // 
            // connectaddresslabel
            // 
            this.connectaddresslabel.AutoSize = true;
            this.connectaddresslabel.Location = new System.Drawing.Point(27, 38);
            this.connectaddresslabel.Name = "connectaddresslabel";
            this.connectaddresslabel.Size = new System.Drawing.Size(59, 15);
            this.connectaddresslabel.TabIndex = 0;
            this.connectaddresslabel.Text = "Pipe host:";
            // 
            // gatesgroup
            // 
            this.gatesgroup.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gatesgroup.Controls.Add(this.buttonscoregate4);
            this.gatesgroup.Controls.Add(this.buttongate4front);
            this.gatesgroup.Controls.Add(this.buttongate4back);
            this.gatesgroup.Controls.Add(this.buttonscoregate3);
            this.gatesgroup.Controls.Add(this.buttongate3front);
            this.gatesgroup.Controls.Add(this.buttongate3back);
            this.gatesgroup.Controls.Add(this.buttonscoregate2);
            this.gatesgroup.Controls.Add(this.buttongate2front);
            this.gatesgroup.Controls.Add(this.buttongate2back);
            this.gatesgroup.Controls.Add(this.buttonscoregate1);
            this.gatesgroup.Controls.Add(this.buttongate1front);
            this.gatesgroup.Controls.Add(this.buttongate1back);
            this.gatesgroup.Enabled = false;
            this.gatesgroup.Location = new System.Drawing.Point(12, 103);
            this.gatesgroup.Name = "gatesgroup";
            this.gatesgroup.Size = new System.Drawing.Size(581, 120);
            this.gatesgroup.TabIndex = 2;
            this.gatesgroup.TabStop = false;
            this.gatesgroup.Text = " Gates ";
            // 
            // buttonscoregate4
            // 
            this.buttonscoregate4.Location = new System.Drawing.Point(495, 30);
            this.buttonscoregate4.Name = "buttonscoregate4";
            this.buttonscoregate4.Size = new System.Drawing.Size(56, 66);
            this.buttonscoregate4.TabIndex = 19;
            this.buttonscoregate4.TabStop = false;
            this.buttonscoregate4.Text = "Score\r\n1";
            this.buttonscoregate4.UseVisualStyleBackColor = true;
            this.buttonscoregate4.Click += new System.EventHandler(this.buttonscoregate4_Click);
            this.buttonscoregate4.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate4front
            // 
            this.buttongate4front.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate4front.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate4front.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate4front.Location = new System.Drawing.Point(459, 66);
            this.buttongate4front.Name = "buttongate4front";
            this.buttongate4front.Size = new System.Drawing.Size(30, 30);
            this.buttongate4front.TabIndex = 18;
            this.buttongate4front.TabStop = false;
            this.buttongate4front.UseVisualStyleBackColor = true;
            this.buttongate4front.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate4front.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate4back
            // 
            this.buttongate4back.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate4back.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate4back.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate4back.Location = new System.Drawing.Point(459, 30);
            this.buttongate4back.Name = "buttongate4back";
            this.buttongate4back.Size = new System.Drawing.Size(30, 30);
            this.buttongate4back.TabIndex = 17;
            this.buttongate4back.TabStop = false;
            this.buttongate4back.UseVisualStyleBackColor = true;
            this.buttongate4back.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate4back.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonscoregate3
            // 
            this.buttonscoregate3.Location = new System.Drawing.Point(351, 30);
            this.buttonscoregate3.Name = "buttonscoregate3";
            this.buttonscoregate3.Size = new System.Drawing.Size(56, 66);
            this.buttonscoregate3.TabIndex = 16;
            this.buttonscoregate3.TabStop = false;
            this.buttonscoregate3.Text = "Score\r\n4";
            this.buttonscoregate3.UseVisualStyleBackColor = true;
            this.buttonscoregate3.Click += new System.EventHandler(this.buttonscoregate3_Click);
            this.buttonscoregate3.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate3front
            // 
            this.buttongate3front.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate3front.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate3front.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate3front.Location = new System.Drawing.Point(315, 66);
            this.buttongate3front.Name = "buttongate3front";
            this.buttongate3front.Size = new System.Drawing.Size(30, 30);
            this.buttongate3front.TabIndex = 15;
            this.buttongate3front.TabStop = false;
            this.buttongate3front.UseVisualStyleBackColor = true;
            this.buttongate3front.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate3front.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate3back
            // 
            this.buttongate3back.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate3back.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate3back.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate3back.Location = new System.Drawing.Point(315, 30);
            this.buttongate3back.Name = "buttongate3back";
            this.buttongate3back.Size = new System.Drawing.Size(30, 30);
            this.buttongate3back.TabIndex = 14;
            this.buttongate3back.TabStop = false;
            this.buttongate3back.UseVisualStyleBackColor = true;
            this.buttongate3back.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate3back.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonscoregate2
            // 
            this.buttonscoregate2.Location = new System.Drawing.Point(205, 30);
            this.buttonscoregate2.Name = "buttonscoregate2";
            this.buttonscoregate2.Size = new System.Drawing.Size(56, 66);
            this.buttonscoregate2.TabIndex = 13;
            this.buttonscoregate2.TabStop = false;
            this.buttonscoregate2.Text = "Score\r\n3";
            this.buttonscoregate2.UseVisualStyleBackColor = true;
            this.buttonscoregate2.Click += new System.EventHandler(this.buttonscoregate2_Click);
            this.buttonscoregate2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate2front
            // 
            this.buttongate2front.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate2front.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate2front.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate2front.Location = new System.Drawing.Point(169, 66);
            this.buttongate2front.Name = "buttongate2front";
            this.buttongate2front.Size = new System.Drawing.Size(30, 30);
            this.buttongate2front.TabIndex = 12;
            this.buttongate2front.TabStop = false;
            this.buttongate2front.UseVisualStyleBackColor = true;
            this.buttongate2front.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate2front.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate2back
            // 
            this.buttongate2back.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate2back.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate2back.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate2back.Location = new System.Drawing.Point(169, 30);
            this.buttongate2back.Name = "buttongate2back";
            this.buttongate2back.Size = new System.Drawing.Size(30, 30);
            this.buttongate2back.TabIndex = 11;
            this.buttongate2back.TabStop = false;
            this.buttongate2back.UseVisualStyleBackColor = true;
            this.buttongate2back.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate2back.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttonscoregate1
            // 
            this.buttonscoregate1.Location = new System.Drawing.Point(66, 30);
            this.buttonscoregate1.Name = "buttonscoregate1";
            this.buttonscoregate1.Size = new System.Drawing.Size(56, 66);
            this.buttonscoregate1.TabIndex = 10;
            this.buttonscoregate1.TabStop = false;
            this.buttonscoregate1.Text = "Score\r\n2";
            this.buttonscoregate1.UseVisualStyleBackColor = true;
            this.buttonscoregate1.Click += new System.EventHandler(this.buttonscoregate1_Click);
            this.buttonscoregate1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate1front
            // 
            this.buttongate1front.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate1front.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate1front.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate1front.Location = new System.Drawing.Point(30, 66);
            this.buttongate1front.Name = "buttongate1front";
            this.buttongate1front.Size = new System.Drawing.Size(30, 30);
            this.buttongate1front.TabIndex = 7;
            this.buttongate1front.TabStop = false;
            this.buttongate1front.UseVisualStyleBackColor = true;
            this.buttongate1front.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate1front.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // buttongate1back
            // 
            this.buttongate1back.BackgroundImage = global::IOModuleSimulator.Properties.Resources.led_off;
            this.buttongate1back.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttongate1back.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttongate1back.Location = new System.Drawing.Point(30, 30);
            this.buttongate1back.Name = "buttongate1back";
            this.buttongate1back.Size = new System.Drawing.Size(30, 30);
            this.buttongate1back.TabIndex = 6;
            this.buttongate1back.TabStop = false;
            this.buttongate1back.UseVisualStyleBackColor = true;
            this.buttongate1back.Click += new System.EventHandler(this.SensorButton_Click);
            this.buttongate1back.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoseFocus);
            // 
            // animateSensorTimer
            // 
            this.animateSensorTimer.Interval = 50;
            this.animateSensorTimer.Tick += new System.EventHandler(this.animateSensorTimer_Tick);
            // 
            // focusbox
            // 
            this.focusbox.Location = new System.Drawing.Point(998, 178);
            this.focusbox.Name = "focusbox";
            this.focusbox.Size = new System.Drawing.Size(71, 81);
            this.focusbox.TabIndex = 3;
            this.focusbox.TabStop = false;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(605, 486);
            this.Controls.Add(this.focusbox);
            this.Controls.Add(this.gatesgroup);
            this.Controls.Add(this.connectiongroup);
            this.Controls.Add(this.bridgegroup);
            this.Controls.Add(this.buttonsgroup);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.KeyPreview = true;
            this.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.MaximizeBox = false;
            this.Name = "FormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Pandemic IOModule Pretender";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.FormMain_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.FormMain_KeyUp);
            this.buttonsgroup.ResumeLayout(false);
            this.bridgegroup.ResumeLayout(false);
            this.connectiongroup.ResumeLayout(false);
            this.connectiongroup.PerformLayout();
            this.gatesgroup.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.focusbox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox buttonsgroup;
        private System.Windows.Forms.GroupBox bridgegroup;
        private System.Windows.Forms.GroupBox connectiongroup;
        private System.Windows.Forms.TextBox connectpipe;
        private System.Windows.Forms.Label connectpipelabel;
        private System.Windows.Forms.TextBox connectaddress;
        private System.Windows.Forms.Label connectaddresslabel;
        private System.Windows.Forms.Button connectbutton;
        private System.Windows.Forms.Button buttonleft;
        private System.Windows.Forms.Button buttonok;
        private System.Windows.Forms.Button buttonright;
        private System.Windows.Forms.Button buttoncancel;
        private System.Windows.Forms.GroupBox gatesgroup;
        private System.Windows.Forms.Button buttonbridgeback;
        private System.Windows.Forms.Button buttonbridgefront;
        private System.Windows.Forms.Button buttonstartslide;
        private System.Windows.Forms.Button buttonbridgeblocked;
        private System.Windows.Forms.Button buttonremovepuck;
		private System.Windows.Forms.Button buttonbridgecleared;
		private System.Windows.Forms.Button buttongate1front;
		private System.Windows.Forms.Button buttongate1back;
		private System.Windows.Forms.Button buttonscoregate4;
		private System.Windows.Forms.Button buttongate4front;
		private System.Windows.Forms.Button buttongate4back;
		private System.Windows.Forms.Button buttonscoregate3;
		private System.Windows.Forms.Button buttongate3front;
		private System.Windows.Forms.Button buttongate3back;
		private System.Windows.Forms.Button buttonscoregate2;
		private System.Windows.Forms.Button buttongate2front;
		private System.Windows.Forms.Button buttongate2back;
		private System.Windows.Forms.Button buttonscoregate1;
		private System.Windows.Forms.Timer animateSensorTimer;
        private System.Windows.Forms.PictureBox focusbox;
    }
}

