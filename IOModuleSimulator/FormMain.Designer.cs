namespace IOModuleSimulator
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonsgroup = new System.Windows.Forms.GroupBox();
            this.sensorsgroup = new System.Windows.Forms.GroupBox();
            this.connectiongroup = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.connectaddress = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.connectport = new System.Windows.Forms.TextBox();
            this.connectbutton = new System.Windows.Forms.Button();
            this.connectiongroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonsgroup
            // 
            this.buttonsgroup.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonsgroup.Enabled = false;
            this.buttonsgroup.Location = new System.Drawing.Point(12, 115);
            this.buttonsgroup.Name = "buttonsgroup";
            this.buttonsgroup.Size = new System.Drawing.Size(581, 149);
            this.buttonsgroup.TabIndex = 0;
            this.buttonsgroup.TabStop = false;
            this.buttonsgroup.Text = " Buttons ";
            // 
            // sensorsgroup
            // 
            this.sensorsgroup.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.sensorsgroup.Enabled = false;
            this.sensorsgroup.Location = new System.Drawing.Point(12, 293);
            this.sensorsgroup.Name = "sensorsgroup";
            this.sensorsgroup.Size = new System.Drawing.Size(581, 149);
            this.sensorsgroup.TabIndex = 1;
            this.sensorsgroup.TabStop = false;
            this.sensorsgroup.Text = " Sensors ";
            // 
            // connectiongroup
            // 
            this.connectiongroup.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.connectiongroup.Controls.Add(this.connectbutton);
            this.connectiongroup.Controls.Add(this.connectport);
            this.connectiongroup.Controls.Add(this.label2);
            this.connectiongroup.Controls.Add(this.connectaddress);
            this.connectiongroup.Controls.Add(this.label1);
            this.connectiongroup.Location = new System.Drawing.Point(12, 12);
            this.connectiongroup.Name = "connectiongroup";
            this.connectiongroup.Size = new System.Drawing.Size(581, 85);
            this.connectiongroup.TabIndex = 1;
            this.connectiongroup.TabStop = false;
            this.connectiongroup.Text = " Connection ";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(33, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "TCP address:";
            // 
            // connectaddress
            // 
            this.connectaddress.Location = new System.Drawing.Point(112, 34);
            this.connectaddress.Name = "connectaddress";
            this.connectaddress.Size = new System.Drawing.Size(174, 23);
            this.connectaddress.TabIndex = 1;
            this.connectaddress.Text = "localhost";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(301, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "port:";
            // 
            // connectport
            // 
            this.connectport.Location = new System.Drawing.Point(339, 34);
            this.connectport.Name = "connectport";
            this.connectport.Size = new System.Drawing.Size(84, 23);
            this.connectport.TabIndex = 3;
            this.connectport.Text = "2023";
            // 
            // connectbutton
            // 
            this.connectbutton.Location = new System.Drawing.Point(457, 31);
            this.connectbutton.Name = "connectbutton";
            this.connectbutton.Size = new System.Drawing.Size(92, 28);
            this.connectbutton.TabIndex = 4;
            this.connectbutton.Text = "Connect";
            this.connectbutton.UseVisualStyleBackColor = true;
            this.connectbutton.Click += new System.EventHandler(this.connectbutton_Click);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(605, 473);
            this.Controls.Add(this.connectiongroup);
            this.Controls.Add(this.sensorsgroup);
            this.Controls.Add(this.buttonsgroup);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.MaximizeBox = false;
            this.Name = "FormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Pandemic IOModule Charlatan";
            this.connectiongroup.ResumeLayout(false);
            this.connectiongroup.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox buttonsgroup;
        private System.Windows.Forms.GroupBox sensorsgroup;
        private System.Windows.Forms.GroupBox connectiongroup;
        private System.Windows.Forms.TextBox connectport;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox connectaddress;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button connectbutton;
    }
}

