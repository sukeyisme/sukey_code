namespace UMSExcelAddinUI
{
    partial class FormBase
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormBase));
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonNext = new System.Windows.Forms.Button();
            this.buttonPrev = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.listBox2 = new System.Windows.Forms.ListBox();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.m_Icon_ImageList = new System.Windows.Forms.ImageList(this.components);
            this.panel2 = new System.Windows.Forms.Panel();
            this.m_listViewQuota = new System.Windows.Forms.ListView();
            this.序号 = new System.Windows.Forms.ColumnHeader();
            this.指标名称 = new System.Windows.Forms.ColumnHeader();
            this.指标公式 = new System.Windows.Forms.ColumnHeader();
            this.listBox3 = new System.Windows.Forms.ListBox();
            this.treeView2 = new System.Windows.Forms.TreeView();
            this.panel3 = new System.Windows.Forms.Panel();
            this.listBox4 = new System.Windows.Forms.ListBox();
            this.panel0 = new System.Windows.Forms.Panel();
            this.listBox5 = new System.Windows.Forms.ListBox();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel0.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(431, 262);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "取消";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonNext
            // 
            this.buttonNext.Location = new System.Drawing.Point(330, 262);
            this.buttonNext.Name = "buttonNext";
            this.buttonNext.Size = new System.Drawing.Size(75, 23);
            this.buttonNext.TabIndex = 1;
            this.buttonNext.Text = "下一步";
            this.buttonNext.UseVisualStyleBackColor = true;
            this.buttonNext.Click += new System.EventHandler(this.buttonNext_Click);
            // 
            // buttonPrev
            // 
            this.buttonPrev.Location = new System.Drawing.Point(232, 262);
            this.buttonPrev.Name = "buttonPrev";
            this.buttonPrev.Size = new System.Drawing.Size(75, 23);
            this.buttonPrev.TabIndex = 0;
            this.buttonPrev.Text = "上一步";
            this.buttonPrev.UseVisualStyleBackColor = true;
            this.buttonPrev.Click += new System.EventHandler(this.buttonPrev_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.button4);
            this.panel1.Controls.Add(this.button3);
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.listBox2);
            this.panel1.Controls.Add(this.listBox1);
            this.panel1.Controls.Add(this.treeView1);
            this.panel1.Location = new System.Drawing.Point(12, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(610, 244);
            this.panel1.TabIndex = 0;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(384, 149);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(26, 23);
            this.button4.TabIndex = 7;
            this.button4.Text = "<<";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(384, 111);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(26, 23);
            this.button3.TabIndex = 6;
            this.button3.Text = "<";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(384, 67);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(26, 23);
            this.button2.TabIndex = 5;
            this.button2.Text = ">>";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(384, 29);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(26, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = ">";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // listBox2
            // 
            this.listBox2.FormattingEnabled = true;
            this.listBox2.ItemHeight = 12;
            this.listBox2.Location = new System.Drawing.Point(419, 16);
            this.listBox2.Name = "listBox2";
            this.listBox2.Size = new System.Drawing.Size(177, 196);
            this.listBox2.TabIndex = 3;
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 12;
            this.listBox1.Location = new System.Drawing.Point(198, 15);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(177, 196);
            this.listBox1.TabIndex = 2;
            // 
            // treeView1
            // 
            this.treeView1.ImageIndex = 0;
            this.treeView1.ImageList = this.m_Icon_ImageList;
            this.treeView1.Location = new System.Drawing.Point(9, 15);
            this.treeView1.Name = "treeView1";
            this.treeView1.SelectedImageIndex = 0;
            this.treeView1.Size = new System.Drawing.Size(177, 196);
            this.treeView1.TabIndex = 1;
            // 
            // m_Icon_ImageList
            // 
            this.m_Icon_ImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("m_Icon_ImageList.ImageStream")));
            this.m_Icon_ImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.m_Icon_ImageList.Images.SetKeyName(0, "1.ico");
            this.m_Icon_ImageList.Images.SetKeyName(1, "2.ico");
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.m_listViewQuota);
            this.panel2.Controls.Add(this.listBox3);
            this.panel2.Controls.Add(this.treeView2);
            this.panel2.Location = new System.Drawing.Point(678, 41);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(595, 244);
            this.panel2.TabIndex = 1;
            // 
            // m_listViewQuota
            // 
            this.m_listViewQuota.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.序号,
            this.指标名称,
            this.指标公式});
            this.m_listViewQuota.Location = new System.Drawing.Point(219, 15);
            this.m_listViewQuota.Name = "m_listViewQuota";
            this.m_listViewQuota.Size = new System.Drawing.Size(267, 200);
            this.m_listViewQuota.TabIndex = 4;
            this.m_listViewQuota.UseCompatibleStateImageBehavior = false;
            this.m_listViewQuota.View = System.Windows.Forms.View.Details;
            // 
            // 指标名称
            // 
            this.指标名称.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.指标名称.Width = 100;
            // 
            // 指标公式
            // 
            this.指标公式.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.指标公式.Width = 120;
            // 
            // listBox3
            // 
            this.listBox3.FormattingEnabled = true;
            this.listBox3.ItemHeight = 12;
            this.listBox3.Location = new System.Drawing.Point(492, 19);
            this.listBox3.Name = "listBox3";
            this.listBox3.Size = new System.Drawing.Size(100, 196);
            this.listBox3.TabIndex = 3;
            // 
            // treeView2
            // 
            this.treeView2.Location = new System.Drawing.Point(19, 15);
            this.treeView2.Name = "treeView2";
            this.treeView2.Size = new System.Drawing.Size(194, 200);
            this.treeView2.TabIndex = 2;
            this.treeView2.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.treeView2_MouseDoubleClick);
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.listBox4);
            this.panel3.Location = new System.Drawing.Point(27, 382);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(595, 241);
            this.panel3.TabIndex = 1;
            // 
            // listBox4
            // 
            this.listBox4.FormattingEnabled = true;
            this.listBox4.ItemHeight = 12;
            this.listBox4.Location = new System.Drawing.Point(134, 51);
            this.listBox4.Name = "listBox4";
            this.listBox4.Size = new System.Drawing.Size(331, 160);
            this.listBox4.TabIndex = 0;
            // 
            // panel0
            // 
            this.panel0.Controls.Add(this.listBox5);
            this.panel0.Location = new System.Drawing.Point(768, 382);
            this.panel0.Name = "panel0";
            this.panel0.Size = new System.Drawing.Size(465, 241);
            this.panel0.TabIndex = 3;
            // 
            // listBox5
            // 
            this.listBox5.FormattingEnabled = true;
            this.listBox5.ItemHeight = 12;
            this.listBox5.Location = new System.Drawing.Point(115, 51);
            this.listBox5.Name = "listBox5";
            this.listBox5.Size = new System.Drawing.Size(247, 148);
            this.listBox5.TabIndex = 0;
            // 
            // FormBase
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1304, 736);
            this.Controls.Add(this.panel0);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.buttonNext);
            this.Controls.Add(this.buttonPrev);
            this.Name = "FormBase";
            this.Text = "FormBase";
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.panel0.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonNext;
        private System.Windows.Forms.Button buttonPrev;
        public System.Windows.Forms.Panel panel3;
        public System.Windows.Forms.Panel panel1;
        public System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListBox listBox2;
        private System.Windows.Forms.ListBox listBox1;
        public System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.ListBox listBox3;
        private System.Windows.Forms.TreeView treeView2;
        private System.Windows.Forms.ListBox listBox4;
        private System.Windows.Forms.ImageList m_Icon_ImageList;
        private System.Windows.Forms.ListBox listBox5;
        public System.Windows.Forms.Panel panel0;
        private System.Windows.Forms.ListView m_listViewQuota;
        private System.Windows.Forms.ColumnHeader 序号;
        private System.Windows.Forms.ColumnHeader 指标名称;
        private System.Windows.Forms.ColumnHeader 指标公式;
    }
}