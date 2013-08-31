using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace UMSExcelAddinUI
{
    public partial class FormBase : Form
    {
        public ArrayList NodeList = null;
        public FormBase()
        {
            InitializeComponent();
        }
        public WizardController controller = null;

        public void DisableButton()
        {
            if (this.controller == null)
                return;
            if (this.controller.IsFirstForm)
            {
                this.buttonPrev.Enabled = false;
            }
            else
            {
                this.buttonPrev.Enabled = true;
            }
            if (this.controller.IsLastForm) //如果是向导的最后一个页面，显示“完成”
            {
                this.buttonNext.Text = "完成";
            }
            else    //否则显示“下一步”
            {
                this.buttonNext.Text = "下一步";
            }
        }
        protected virtual void UpdateInfo()  //由“每一步FormStep1，FormStep2…”的界面 具体实现
        {

        }
        protected virtual void GoNext()
        {
            UpdateInfo();
            if (this.controller.IsLastForm) //如果是向导的最后一个页面，则调用“FinishWizard()”
            {
                controller.FinishWizard();
                this.Visible = false;
            }
            else//如果是向导的最后一个页面，则调用“GoNext()”
            {
                controller.GoNext();
            }
        }
        protected virtual void GoPrev()
        {
            UpdateInfo();
            controller.GoPrev();
        }

        protected virtual void Cancel()
        {
            this.controller = null;
            this.Close();
        }

        private void buttonPrev_Click(object sender, EventArgs e)
        {
            GoPrev();
        }

        private void buttonNext_Click(object sender, EventArgs e)
        {
           GoNext();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
           Cancel();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        //根据读取的指标信息 初始化 指标树
        public void InitQuotaTree(long rptType, long lParentID, TreeNode pNode)
        {
            if (NodeList == null)
                return;
            ums_rptfunc rptIndex;
            for (int i=0; i < NodeList.Count; ++i)
            {
                rptIndex = (ums_rptfunc)NodeList[i];
                TreeNode node = new TreeNode(); 
                if (rptType == rptIndex.RPTTYPE && lParentID == rptIndex.PARENTID)
                {
                    if (pNode == null)
                    {
                        node.Text = rptIndex.FUNCNAME;
                        
                        //node.ImageIndex = (int)rptIndex.ICONIDX;
                        node.ImageIndex = rptIndex.ISGROUP==1?0:1;
                        node.Tag = rptIndex;
                        this.treeView2.Nodes.Add(node);
                        InitQuotaTree(rptType, rptIndex.FUNCID, rptIndex.ISGROUP==1?node:null);
                    }
                    else
                    {
                        node.Text = rptIndex.FUNCNAME;
                        node.ImageIndex = rptIndex.ISGROUP==1?0:1;
                        node.Tag = rptIndex;
                        pNode.Nodes.Add(node);
                        if (i >= NodeList.Count) return;
                        if(rptIndex.ISGROUP==1)
                            InitQuotaTree(rptType, rptIndex.FUNCID,node);
                    }
                }
            }
            this.treeView2.Nodes[0].ExpandAll();
            //ums_rptfunc data = (ums_rptfunc)this.treeView1.Nodes[0].Tag;
            return;
        }

        //初始化指标信息列表
        public void InitQuotaList()
        {
            //m_listViewQuota
            return;
        }

        //双击指标树节点添加指标信息到 指标信息列表中
        private void treeView2_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            TreeNode SelectedNode = this.treeView2.SelectedNode;
            ums_rptfunc data = (ums_rptfunc)SelectedNode.Tag;
            if (0 == data.ISGROUP)
            {
                ListViewItem ListItem = new ListViewItem();
                ListItem.SubItems[0].Text = this.m_listViewQuota.Items.Count.ToString();
                ListItem.SubItems[1].Text = data.FUNCNAME;
                ListItem.SubItems[2].Text = "=" + data.FUNCNAME;
                this.m_listViewQuota.Items.Add(ListItem);
            }
        }
    }
}
