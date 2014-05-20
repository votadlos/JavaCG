package demo.hello;


/**
 * Generated from IDL interface "GoodDay".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 10:20:27 PM
 */

public abstract class GoodDayHelper
{
	private volatile static org.omg.CORBA.TypeCode _type;
	public static org.omg.CORBA.TypeCode type ()
	{
		if (_type == null)
		{
			synchronized(GoodDayHelper.class)
			{
				if (_type == null)
				{
					_type = org.omg.CORBA.ORB.init().create_interface_tc("IDL:demo/hello/GoodDay:1.0", "GoodDay");
				}
			}
		}
		return _type;
	}

	public static void insert (final org.omg.CORBA.Any any, final demo.hello.GoodDay s)
	{
			any.insert_Object(s);
	}
	public static demo.hello.GoodDay extract(final org.omg.CORBA.Any any)
	{
		return narrow(any.extract_Object()) ;
	}
	public static String id()
	{
		return "IDL:demo/hello/GoodDay:1.0";
	}
	public static GoodDay read(final org.omg.CORBA.portable.InputStream in)
	{
		return narrow(in.read_Object(demo.hello._GoodDayStub.class));
	}
	public static void write(final org.omg.CORBA.portable.OutputStream _out, final demo.hello.GoodDay s)
	{
		_out.write_Object(s);
	}
	public static demo.hello.GoodDay narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.hello.GoodDay)
		{
			return (demo.hello.GoodDay)obj;
		}
		else if (obj._is_a("IDL:demo/hello/GoodDay:1.0"))
		{
			demo.hello._GoodDayStub stub;
			stub = new demo.hello._GoodDayStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
		else
		{
			throw new org.omg.CORBA.BAD_PARAM("Narrow failed");
		}
	}
	public static demo.hello.GoodDay unchecked_narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.hello.GoodDay)
		{
			return (demo.hello.GoodDay)obj;
		}
		else
		{
			demo.hello._GoodDayStub stub;
			stub = new demo.hello._GoodDayStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
	}
}
