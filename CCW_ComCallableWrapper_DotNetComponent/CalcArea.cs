using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace CalcAreaCCW
{
    [ClassInterface(ClassInterfaceType.AutoDual)]
    public class CalcArea
    {
        public double AreaValue { get; set; }
        public CalcArea()
        {

        }
        public double CalcCircleArea(int radius)
        {
            AreaValue = 3.14 * radius * radius; //pi r square
            MessageBox.Show("Area of circle is " + AreaValue);
            return AreaValue;
        }
    }
}