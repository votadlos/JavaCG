package demo.hello;

/**
 * Generated from IDL interface "GoodDay".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 10:20:27 PM
 */

public final class GoodDayHolder	implements org.omg.CORBA.portable.Streamable{
	 public GoodDay value;
	public GoodDayHolder()
	{
	}
	public GoodDayHolder (final GoodDay initial)
	{
		value = initial;
	}
	public org.omg.CORBA.TypeCode _type()
	{
		return GoodDayHelper.type();
	}
	public void _read (final org.omg.CORBA.portable.InputStream in)
	{
		value = GoodDayHelper.read (in);
	}
	public void _write (final org.omg.CORBA.portable.OutputStream _out)
	{
		GoodDayHelper.write (_out,value);
	}
}
