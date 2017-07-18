using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Text2Image
{
    public partial class Form1 : Form
    {
        private List<string> LABEL; 
        //public Image image;// 具体这张图是从文件读取还是从picturebox什么的获取你来指定
        private delegate void FlushClient(); //代理
        Thread thread = null;
        private String lastFile = "";
        private String[] testFonts = { "黑体", "方正姚体", "微软雅黑", "华文宋体", "宋体", "华文细黑", "华文楷体", "YouYuan", "MingLiU", "SimSun-ExtB", "Video Terminal Screen"};

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;
            thread = new Thread(CrossThreadFlush);
            thread.IsBackground = true;
            thread.Start();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //thread.Suspend();
            //button1.Enabled = true;
        }

        public Image MakeGrayscale(Image image)
        {
            // source: http://www.switchonthecode.com/tutorials/csharp-tutorial-convert-a-color-image-to-grayscale

            //create a blank bitmap the same size as original
            Bitmap newBitmap = new Bitmap(image.Width, image.Height);

            //get a graphics object from the new image
            Graphics g = Graphics.FromImage(newBitmap);

            //create the grayscale ColorMatrix
            ColorMatrix colorMatrix = new ColorMatrix(
               new float[][]
                  {
                     new float[] {.3f, .3f, .3f, 0, 0},
                     new float[] {.59f, .59f, .59f, 0, 0},
                     new float[] {.11f, .11f, .11f, 0, 0},
                     new float[] {0, 0, 0, 1, 0},
                     new float[] {0, 0, 0, 0, 1}
                  });

            //create some image attributes
            ImageAttributes attributes = new ImageAttributes();

            //set the color matrix attribute
            attributes.SetColorMatrix(colorMatrix);

            //draw the original image on the new image
            //using the grayscale color matrix
            g.DrawImage(image, new Rectangle(0, 0, image.Width, image.Height),
               0, 0, image.Width, image.Height, GraphicsUnit.Pixel, attributes);

            //dispose the Graphics object
            g.Dispose();
            attributes.Dispose();

            return newBitmap;
        }

        private void CrossThreadFlush()
        {
            Random random = new Random();
            List<string> sbTrain = new List<string>();
            List<string> sbTest = new List<string>();

            int iflag = 1;
            string strdir;
            for(int i = 0; i < testFonts.Length; i++)
            {
                for (int j = 0; j < LABEL.Count(); j++)
                {
                    for (int x = 0; x < 3; x++)
                    {
                        for(int y = 0; y < 3; y++)
                        {
                            iflag++;
                            int fsize = 17;
                            Image image = Image.FromFile("template.png");
                            using (Graphics g = Graphics.FromImage(image))
                            {
                                g.DrawString(LABEL[j], new Font(testFonts[i], fsize), Brushes.White, new PointF(x, y));
                                g.Flush();
                            }
                            Image grayimage = MakeGrayscale(image);
                            if (iflag % 10 == 0)
                            {                                
                                sbTest.Insert(random.Next(sbTest.Count), string.Format(@"test\{0}_{1}.png {2}", j, iflag, j));
                                strdir = string.Format(@"C:\Caffe\caffe-windows\examples\ocr_chinese\test\{0}", j);
                                if (!System.IO.Directory.Exists(strdir))
                                {
                                    System.IO.Directory.CreateDirectory(strdir);
                                }
                                lastFile = string.Format(@"{0}\{1}_{2}.jpg", strdir,  j, iflag);
                                grayimage.Save(lastFile);
                            }
                            else
                            {
                                sbTrain.Insert(random.Next(sbTrain.Count), string.Format(@"train\{0}_{1}.png {2}", j, iflag, j));
                                
                                strdir = string.Format(@"C:\Caffe\caffe-windows\examples\ocr_chinese\train\{0}", j);
                                if (!System.IO.Directory.Exists(strdir))
                                {
                                    System.IO.Directory.CreateDirectory(strdir);
                                }
                                lastFile = string.Format(@"{0}\{1}_{2}.jpg", strdir, j, iflag);
                                grayimage.Save(lastFile);
                            }
                            image.Dispose();
                            grayimage.Dispose();

                            //将sleep和无限循环放在等待异步的外面
                            //Thread.Sleep(1000);
                            if (j % 50 == 0)
                            {
                                ThreadFunction();
                            }
                        }
                    }               
                }
            }
            //File.WriteAllLines(@"C:\Caffe\caffe-windows\examples\ocr_chinese\train.txt", sbTrain);
            //File.WriteAllLines(@"C:\Caffe\caffe-windows\examples\ocr_chinese\test.txt", sbTest);

            MessageBox.Show("All Done!");
        }

        private void ThreadFunction()
        {
            if (this.textBox1.InvokeRequired)//等待异步
            {
                FlushClient fc = new FlushClient(ThreadFunction);
                this.Invoke(fc); //通过代理调用刷新方法
            }
            else
            {                
                this.textBox1.AppendText(lastFile);
                textBox1.AppendText("\r\n");
                pictureBox2.Image = Image.FromFile(lastFile);
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                LABEL = File.ReadAllLines("label.txt", Encoding.UTF8).ToList();
                //this.LABEL = File.ReadAllText("label.txt").Trim();
                //String newLab = File.ReadAllText("res.txt").Trim();
                string[] allReslines = File.ReadAllLines("res.txt", Encoding.UTF8);
                if (allReslines.Length > 0)
                {
                    foreach (var line in allReslines)
                    {
                        //int ilen = line.Length > 3 ? 3 : line.Length;
                        //string vlogo = line.Substring(0, ilen);
                        if (!this.LABEL.Contains(line))
                        {
                            this.LABEL.Add(line);
                        }
                    }
                    //File.WriteAllLines("label.txt", LABEL, Encoding.UTF8);
                    //File.WriteAllText("res.txt", "", Encoding.ASCII);
                }
                
                pictureBox1.Image = Image.FromFile("template.png");
            } catch(Exception ex)
            {
                this.textBox1.Text = ex.Message;
            }

        }
    }
}
