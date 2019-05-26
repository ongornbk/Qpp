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
            System.Windows.Forms.MessageBox.Show("Dispose", "Dispose");
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
            System.Windows.Forms.MessageBox.Show("InitializeComponent", "InitializeComponent");
            components = new System.ComponentModel.Container();
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
        }

        #endregion
    }

    public class MainClass
    {

        [DllExport(CallingConvention = System.Runtime.InteropServices.CallingConvention.StdCall, ExportName = "test")]
        public static int test()
        {
            System.Windows.Forms.MessageBox.Show("test","test");

            return 0;
        }

        [DllExport(CallingConvention = System.Runtime.InteropServices.CallingConvention.StdCall, ExportName = "start")]
        public static int start(int arg)
        {
            System.Windows.Forms.MessageBox.Show("start", "start");
            return 0;
        }

        [DllExport(CallingConvention = System.Runtime.InteropServices.CallingConvention.StdCall, ExportName = "close")]
        public static int close(int arg)
        {
            System.Windows.Forms.MessageBox.Show("close", "close");
            return 0;
        }

        [DllExport(CallingConvention = System.Runtime.InteropServices.CallingConvention.StdCall, ExportName = "foo_count")]
       //[return: Unmana]
        public static int foo_count(int arg)
        {
            System.Windows.Forms.MessageBox.Show("foo_count", "foo_count");
            return 0;
        }

        [DllExport(CallingConvention = System.Runtime.InteropServices.CallingConvention.StdCall, ExportName = "get_foo")]
        public static int get_foo(int arg)
        {
            System.Windows.Forms.MessageBox.Show("get_foo", "get_foo");
            return 0;
        }

    }
}
