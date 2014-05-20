package demo.ssl;


/**
 * Generated from IDL interface "SSLDemo".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:35 PM
 */

public abstract class SSLDemoHelper
{
	private volatile static org.omg.CORBA.TypeCode _type;
	public static org.omg.CORBA.TypeCode type ()
	{
		if (_type == null)
		{
			synchronized(SSLDemoHelper.class)
			{
				if (_type == null)
				{
					_type = org.omg.CORBA.ORB.init().create_interface_tc("IDL:demo/ssl/SSLDemo:1.0", "SSLDemo");
				}
			}
		}
		return _type;
	}

	public static void insert (final org.omg.CORBA.Any any, final demo.ssl.SSLDemo s)
	{
			any.insert_Object(s);
	}
	public static demo.ssl.SSLDemo extract(final org.omg.CORBA.Any any)
	{
		return narrow(any.extract_Object()) ;
	}
	public static String id()
	{
		return "IDL:demo/ssl/SSLDemo:1.0";
	}
	public static SSLDemo read(final org.omg.CORBA.portable.InputStream in)
	{
		return narrow(in.read_Object(demo.ssl._SSLDemoStub.class));
	}
	public static void write(final org.omg.CORBA.portable.OutputStream _out, final demo.ssl.SSLDemo s)
	{
		_out.write_Object(s);
	}
	public static demo.ssl.SSLDemo narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.ssl.SSLDemo)
		{
			return (demo.ssl.SSLDemo)obj;
		}
		else if (obj._is_a("IDL:demo/ssl/SSLDemo:1.0"))
		{
			demo.ssl._SSLDemoStub stub;
			stub = new demo.ssl._SSLDemoStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
		else
		{
			throw new org.omg.CORBA.BAD_PARAM("Narrow failed");
		}
	}
	public static demo.ssl.SSLDemo unchecked_narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.ssl.SSLDemo)
		{
			return (demo.ssl.SSLDemo)obj;
		}
		else
		{
			demo.ssl._SSLDemoStub stub;
			stub = new demo.ssl._SSLDemoStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
	}
}
