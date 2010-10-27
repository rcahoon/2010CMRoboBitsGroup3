package image.format;

public class Lab
{
	private double l, a, b;
	
	public static void main(String[] args)
	{
		int x1 = Integer.parseInt(args[0]);
		int x2 = Integer.parseInt(args[1]);
		int x3 = Integer.parseInt(args[2]);
		
		XYZ m = yuv2xyz(new YUV(x1, x2, x3));
		System.out.println(m.getX() + " " + m.getY() + " " + m.getZ());
		
		Lab m2 = xyz2lab(m);
		System.out.println(m2.getL() + " " + m2.getA() + " " + m2.getB());
		
		XYZ m3 = lab2xyz(m2);
		System.out.println(m3.getX() + " " + m3.getY() + " " + m3.getZ());
		
		YUV o = xyz2yuv(m3);
		
		System.out.println(o.getY() + " " + o.getU() + " " + o.getV());
	}
	
	public Lab()
	{
		this.l = 0;
		this.a = 0;
		this.b = 0;
	}
	public Lab(double l, double a, double b)
	{
		/*if ((l < 0) || (l > 100)) {
		throw new IllegalArgumentException("L* should be within 0 and 100: " + l);
		}
		if (a < 0) {
		throw new IllegalArgumentException("a* should be non-negative: " + a);
		}
		if (b < 0) {
		throw new IllegalArgumentException("b* should be non-negative" + b);
		}*/
		
		this.l = l;
		this.a = a;
		this.b = b;
	}
	public Lab(Lab o)
	{
		this.l = o.l;
		this.a = o.a;
		this.b = o.b;
	}
	public Lab(YUV yuv)
	{
		this(xyz2lab(yuv2xyz(yuv)));
	}
	public Lab(RGB rgb)
	{
		this(xyz2lab(rgb2xyz(rgb.getRed(), rgb.getGreen(), rgb.getBlue())));
	}
	
	public RGB toRGB()
	{
		return xyz2rgb(lab2xyz(this));
	}
	public YUV toYUV()
	{
		return xyz2yuv(lab2xyz(this));
	}
	
	public double getL()
	{
		return l;
	}
	
	public double getA()
	{
		return a;
	}
	
	public double getB()
	{
		return b;
	}

	private static final double REF_X = 95.047; // Observer= 2°, Illuminant= D65
	private static final double REF_Y = 100.000;
	private static final double REF_Z = 108.883;

	private static XYZ yuv2xyz(YUV yuv)
	{
		int y_, u_, v_;
		
		y_ = yuv.getY();
		u_ = yuv.getV() * 2 - 255;
		v_ = yuv.getU() * 2 - 255;
		
		double r = y_ + u_;
		double g = y_ - 0.51*u_ - 0.19*v_;
		double b = y_ + v_;
		
		return rgb2xyz(r,g,b);
	}
	private static XYZ rgb2xyz(double R, double G, double B)
	{
		//R from 0 to 255
		//G from 0 to 255
		//B from 0 to 255
		double r = R/255.0;
		double g = G/255.0;
		double b = B/255.0;
		
		if (r > 0.04045) r = Math.pow((r + 0.055) / 1.055, 2.4);
		else r = r / 12.92;
		
		if (g > 0.04045) g = Math.pow((g + 0.055) / 1.055, 2.4);
		else g = g / 12.92;
		
		if (b > 0.04045) b = Math.pow((b + 0.055) / 1.055, 2.4);
		else b = b / 12.92;
		
		r = r * 100;
		g = g * 100;
		b = b * 100;
		 
		//Observer. = 2°, Illuminant = D65
		XYZ xyz = new XYZ(
			r * 0.4124 + g * 0.3576 + b * 0.1805,
			r * 0.2126 + g * 0.7152 + b * 0.0722,
			r * 0.0193 + g * 0.1192 + b * 0.9505);
		 
		return xyz;
	}
	private static Lab xyz2lab(XYZ xyz)
	{
		double x = xyz.getX() / REF_X;
		double y = xyz.getY() / REF_Y;
		double z = xyz.getZ() / REF_Z;
		
		if (x > 0.008856) x = Math.pow(x, 1.0/3.0);
		else x = (7.787 * x) + (16.0/116.0);
		
		if (y > 0.008856) y = Math.pow(y, 1.0/3.0);
		else y = (7.787 * y) + (16.0/116.0);
		
		if (z > 0.008856) z = Math.pow(z, 1.0/3.0);
		else z = (7.787 * z) + (16.0/116.0);
		
		Lab lab = new Lab(
			(116 * y) - 16,
			500 * (x - y),
			200 * (y - z) );
		 
		return lab;
	}
	private static XYZ lab2xyz(Lab lab)
	{
		double y = (lab.getL() + 16) / 116.0;
		double x = lab.getA() / 500.0 + y;
		double z = y - lab.getB() / 200.0;
		 
		if (Math.pow(y, 3) > 0.008856) y = Math.pow(y, 3);
		else y = (y - 16 / 116.0) / 7.787;
		
		if (Math.pow(x, 3) > 0.008856) x = Math.pow(x, 3);
		else x = (x - 16 / 116.0) / 7.787;
		
		if (Math.pow(z, 3) > 0.008856) z = Math.pow(z, 3);
		else z = (z - 16 / 116.0) / 7.787;
		 
		XYZ xyz = new XYZ(
			REF_X * x,
			REF_Y * y,
			REF_Z * z);
		 
		return xyz;
	}
	private static RGB xyz2rgb(XYZ xyz)
	{
		//X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
		//Y from 0 to 100.000
		//Z from 0 to 108.883
		double x = xyz.getX() / 100.0;
		double y = xyz.getY() / 100.0;
		double z = xyz.getZ() / 100.0;
		 
		double r = x * 3.2406 + y * -1.5372 + z * -0.4986;
		double g = x * -0.9689 + y * 1.8758 + z * 0.0415;
		double b = x * 0.0557 + y * -0.2040 + z * 1.0570;
		 
		if (r > 0.0031308) r = 1.055 * Math.pow(r, (1.0 / 2.4)) - 0.055;
		else r = 12.92 * r;
		
		if (g > 0.0031308) g = 1.055 * Math.pow(g, (1.0 / 2.4)) - 0.055;
		else g = 12.92 * g;
		
		if (b > 0.0031308) b = 1.055 * Math.pow(b, (1.0 / 2.4)) - 0.055;
		else b = 12.92 * b;
		
		RGB rgb = new RGB(
			(int)bound(Math.round( r * 255 )),
			(int)bound(Math.round( g * 255 )),
			(int)bound(Math.round( b * 255 )) );
		
		return rgb;
	}
	
	private static YUV xyz2yuv(XYZ xyz)
	{
		//X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
		//Y from 0 to 100.000
		//Z from 0 to 108.883
		double x = xyz.getX() / 100.0;
		double y = xyz.getY() / 100.0;
		double z = xyz.getZ() / 100.0;
		 
		double r = x * 3.2406 + y * -1.5372 + z * -0.4986;
		double g = x * -0.9689 + y * 1.8758 + z * 0.0415;
		double b = x * 0.0557 + y * -0.2040 + z * 1.0570;
		 
		if (r > 0.0031308) r = 1.055 * Math.pow(r, (1.0 / 2.4)) - 0.055;
		else r = 12.92 * r;
		
		if (g > 0.0031308) g = 1.055 * Math.pow(g, (1.0 / 2.4)) - 0.055;
		else g = 12.92 * g;
		
		if (b > 0.0031308) b = 1.055 * Math.pow(b, (1.0 / 2.4)) - 0.055;
		else b = 12.92 * b;
		
		r *= 255;
		g *= 255;
		b *= 255;
		
		YUV yuv = new YUV(
			(int)bound(Math.round(0.3*r + 0.582*g + 0.1118*b)),
			(int)bound(Math.round(-0.15*r - 0.2941*g + 0.4441*b + 127.5)),
			(int)bound(Math.round(0.35*r - 0.2941*g - 0.0559*b + 127.5)) );
		
		return yuv;
	}
	
	private static double bound(double value)
	{
		if (value < 0.0) {
			return 0.0;
		}
		if (value > 255.0) {
			return 255.0;
		}
		return value;
	}
	
	private static class XYZ
	{
		private double x, y, z;
		
		public XYZ(double x, double y, double z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}
		
		public double getX()
		{
			return x;
		}
		public double getY()
		{
			return y;
		}
		public double getZ()
		{
			return z;
		}
	}
}
