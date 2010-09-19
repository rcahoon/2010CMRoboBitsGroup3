package core;

import config.*;
import image.format.*;

import java.io.FileWriter;

public class ColorClass extends RGB
{
	public enum VisionObjectType
	{
		Unknown,
		Ball,
		BlueGoalPost,
		YellowGoalPost,
		BlueGoalBar,
		YellowGoalBar,
		Robot,
		Line,
		Corner,
		Circle,
		FreeSpace,
		ConvexHull,
		numTypes
	}
	
	public int yl, yu, ul, uu, vl, vu;
	public VisionObjectType vobj;
	public int minSize;
	
	private ConfigFile configFile;
	private int index;

	public ColorClass(ConfigFile configFile, int index)
	{
		super(0,0,0);
		this.index = index;
		this.configFile = configFile;
		
		reload();
	}
	public ColorClass(ColorClass o)
	{
		super(o.getRed(), o.getGreen(), o.getBlue());
		this.yl = o.yl;
		this.yu = o.yu;
		this.ul = o.ul;
		this.uu = o.uu;
		this.vl = o.vl;
		this.vu = o.vu;
		this.minSize = o.minSize;
		this.vobj = o.vobj;
		this.index = o.index;
		this.configFile = o.configFile;
	}
	
	public void reload()
	{
		super.set(configFile.getInt("vision/classes/" + index + "/red", 0),
		          configFile.getInt("vision/classes/" + index + "/green", 0),
		          configFile.getInt("vision/classes/" + index + "/blue", 0));
	
		vobj = VisionObjectType.class.getEnumConstants()[configFile.getInt("vision/classes/" + index + "/type", 0)];
		yl = configFile.getInt("vision/classes/" + index + "/yl", 0);
		yu = configFile.getInt("vision/classes/" + index + "/yu", 0);
		ul = configFile.getInt("vision/classes/" + index + "/ul", 0);
		uu = configFile.getInt("vision/classes/" + index + "/uu", 0);
		vl = configFile.getInt("vision/classes/" + index + "/vl", 0);
		vu = configFile.getInt("vision/classes/" + index + "/vu", 0);
		minSize = configFile.getInt("vision/classes/" + index + "/minSize", 0);
	}
	
	public boolean match(YUV yuv)
	{
		return match(yuv.getY(), yuv.getU(), yuv.getV());
	}
	public boolean match(int y, int u, int v)
	{
		return (y >= yl && y < yu) && (u >= ul && u < uu) && (v >= vl && v < vu);
	}
	
	public String toString()
	{
		return vobj.toString();
	}
	
	public void save(FileWriter fw) throws java.io.IOException
	{
		fw.write("# " + this.toString() + "\n");
		fw.write("vision/classes/" + index + "/type = " + vobj.ordinal() + "\n");
		fw.write("vision/classes/" + index + "/yl = " + yl + "\n");
		fw.write("vision/classes/" + index + "/yu = " + yu + "\n");
		fw.write("vision/classes/" + index + "/ul = " + ul + "\n");
		fw.write("vision/classes/" + index + "/uu = " + uu + "\n");
		fw.write("vision/classes/" + index + "/vl = " + vl + "\n");
		fw.write("vision/classes/" + index + "/vu = " + vu + "\n");
		fw.write("vision/classes/" + index + "/red = " + getRed() + "\n");
		fw.write("vision/classes/" + index + "/green = " + getGreen() + "\n");
		fw.write("vision/classes/" + index + "/blue = " + getBlue() + "\n");
		fw.write("vision/classes/" + index + "/minSize = " + minSize + "\n");
		fw.write("\n");
		fw.flush();
	}
}
