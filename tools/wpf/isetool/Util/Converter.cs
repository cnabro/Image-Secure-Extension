using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace isetool.Util
{
    unsafe class Converter
    {
        public static sbyte* StringToSbyte(string str)
        {
            byte[] bytes = Encoding.ASCII.GetBytes(str);

            unsafe
            {
                fixed (byte* p = bytes)
                {
                    sbyte* sp = (sbyte*)p;
                    return sp;
                }
            }
        }
    }
}
