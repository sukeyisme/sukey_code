using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;


namespace UMSExcelAddinUI
{
    public class WizardController
    {
        private ArrayList WizardForms = new ArrayList();
        private int curIndex = 0;
        public FormBase formbase = null;
        #region 构造函数
        /// <summary>
        ///构造函数
        /// </summary>
        public WizardController(FormBase fb)
        {
            formbase = fb;
            WizardForms.Add(fb.panel0);
            WizardForms.Add(fb.panel1); //frmStep1继承自FormBase
            WizardForms.Add(fb.panel2);
            WizardForms.Add(fb.panel3);
            fb.controller = this;
            fb.DisableButton();
        }
        #endregion

        #region 自定义函数
        /// <summary>
        /// 自定义函数
        /// </summary>

        public bool IsFirstForm //属性
        {
            get { return curIndex == 0; }
        }
        public bool IsLastForm //属性
        {
            get { return curIndex == this.WizardForms.Count - 1; }
        }

        public void GoNext()
        {
            if (curIndex + 1 < WizardForms.Count)
            {
                curIndex++;
            }
            else
            {
                return;
            }
            ((Panel)WizardForms[curIndex]).Visible = true;
            formbase.DisableButton();
        }

        public void GoPrev()
        {
            if (curIndex - 1 >= 0)
            {
                ((Panel)WizardForms[curIndex]).Visible = false;
                curIndex--;
            }
            else
            {
                return;
            }
            formbase.DisableButton();
        }

        public void BeginWizard()
        {
            formbase.Show();
            
            ((Panel)WizardForms[1]).Visible = false;
            ((Panel)WizardForms[2]).Visible = false;
            ((Panel)WizardForms[3]).Visible = false;
            formbase.DisableButton();
        }

        public void FinishWizard()
        {
            curIndex = 0;
            formbase.Close();
        }
        #endregion
    }
}
