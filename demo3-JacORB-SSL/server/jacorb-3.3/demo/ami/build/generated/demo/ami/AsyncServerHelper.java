package demo.ami;


/**
 * Generated from IDL interface "AsyncServer".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public abstract class AsyncServerHelper
{
	private volatile static org.omg.CORBA.TypeCode _type;
	public static org.omg.CORBA.TypeCode type ()
	{
		if (_type == null)
		{
			synchronized(AsyncServerHelper.class)
			{
				if (_type == null)
				{
					_type = org.omg.CORBA.ORB.init().create_interface_tc("IDL:demo/ami/AsyncServer:1.0", "AsyncServer");
				}
			}
		}
		return _type;
	}

	public static void insert (final org.omg.CORBA.Any any, final demo.ami.AsyncServer s)
	{
			any.insert_Object(s);
	}
	public static demo.ami.AsyncServer extract(final org.omg.CORBA.Any any)
	{
		return narrow(any.extract_Object()) ;
	}
	public static String id()
	{
		return "IDL:demo/ami/AsyncServer:1.0";
	}
	public static AsyncServer read(final org.omg.CORBA.portable.InputStream in)
	{
		return narrow(in.read_Object(demo.ami._AsyncServerStub.class));
	}
	public static void write(final org.omg.CORBA.portable.OutputStream _out, final demo.ami.AsyncServer s)
	{
		_out.write_Object(s);
	}
	public static demo.ami.AsyncServer narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.ami.AsyncServer)
		{
			return (demo.ami.AsyncServer)obj;
		}
		else if (obj._is_a("IDL:demo/ami/AsyncServer:1.0"))
		{
			demo.ami._AsyncServerStub stub;
			stub = new demo.ami._AsyncServerStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
		else
		{
			throw new org.omg.CORBA.BAD_PARAM("Narrow failed");
		}
	}
	public static demo.ami.AsyncServer unchecked_narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.ami.AsyncServer)
		{
			return (demo.ami.AsyncServer)obj;
		}
		else
		{
			demo.ami._AsyncServerStub stub;
			stub = new demo.ami._AsyncServerStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
	}
}
