package demo.ami;


/**
 * Generated from IDL interface "AMI_AsyncServerHandler".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public abstract class AMI_AsyncServerHandlerHelper
{
	private volatile static org.omg.CORBA.TypeCode _type;
	public static org.omg.CORBA.TypeCode type ()
	{
		if (_type == null)
		{
			synchronized(AMI_AsyncServerHandlerHelper.class)
			{
				if (_type == null)
				{
					_type = org.omg.CORBA.ORB.init().create_interface_tc("IDL:demo/ami/AMI_AsyncServerHandler:1.0", "AMI_AsyncServerHandler");
				}
			}
		}
		return _type;
	}

	public static void insert (final org.omg.CORBA.Any any, final demo.ami.AMI_AsyncServerHandler s)
	{
			any.insert_Object(s);
	}
	public static demo.ami.AMI_AsyncServerHandler extract(final org.omg.CORBA.Any any)
	{
		return narrow(any.extract_Object()) ;
	}
	public static String id()
	{
		return "IDL:demo/ami/AMI_AsyncServerHandler:1.0";
	}
	public static AMI_AsyncServerHandler read(final org.omg.CORBA.portable.InputStream in)
	{
		return narrow(in.read_Object(demo.ami._AMI_AsyncServerHandlerStub.class));
	}
	public static void write(final org.omg.CORBA.portable.OutputStream _out, final demo.ami.AMI_AsyncServerHandler s)
	{
		_out.write_Object(s);
	}
	public static demo.ami.AMI_AsyncServerHandler narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.ami.AMI_AsyncServerHandler)
		{
			return (demo.ami.AMI_AsyncServerHandler)obj;
		}
		else if (obj._is_a("IDL:demo/ami/AMI_AsyncServerHandler:1.0"))
		{
			demo.ami._AMI_AsyncServerHandlerStub stub;
			stub = new demo.ami._AMI_AsyncServerHandlerStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
		else
		{
			throw new org.omg.CORBA.BAD_PARAM("Narrow failed");
		}
	}
	public static demo.ami.AMI_AsyncServerHandler unchecked_narrow(final org.omg.CORBA.Object obj)
	{
		if (obj == null)
		{
			return null;
		}
		else if (obj instanceof demo.ami.AMI_AsyncServerHandler)
		{
			return (demo.ami.AMI_AsyncServerHandler)obj;
		}
		else
		{
			demo.ami._AMI_AsyncServerHandlerStub stub;
			stub = new demo.ami._AMI_AsyncServerHandlerStub();
			stub._set_delegate(((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate());
			return stub;
		}
	}
}
