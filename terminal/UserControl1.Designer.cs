using System.Runtime.InteropServices;
using RGiesecke.DllExport;

namespace terminal
{
    partial class UserControl1
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

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
        }

        #endregion
    }

    public static class MainClass
    {

        [DllExport(CallingConvention = System.Runtime.InteropServices.CallingConvention.StdCall, ExportName = "test")]
        public static int test()
        {
            System.Windows.Forms.MessageBox.Show("test","test");

            return 0;
        }

    }
}
