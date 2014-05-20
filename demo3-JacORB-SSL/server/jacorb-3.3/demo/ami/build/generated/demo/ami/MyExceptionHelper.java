package demo.ami;


/**
 * Generated from IDL exception "MyException".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public abstract class MyExceptionHelper
{
	private volatile static org.omg.CORBA.TypeCode _type;
	public static org.omg.CORBA.TypeCode type ()
	{
		if (_type == null)
		{
			synchronized(MyExceptionHelper.class)
			{
				if (_type == null)
				{
					_type = org.omg.CORBA.ORB.init().create_exception_tc(demo.ami.MyExceptionHelper.id(),"MyException",new org.omg.CORBA.StructMember[0]);
				}
			}
		}
		return _type;
	}

	public static void insert (final org.omg.CORBA.Any any, final demo.ami.MyException s)
	{
		any.type(type());
		write( any.create_output_stream(),s);
	}

	public static demo.ami.MyException extract (final org.omg.CORBA.Any any)
	{
		org.omg.CORBA.portable.InputStream in = any.create_input_stream();
		try
		{
			return read (in);
		}
		finally
		{
			try
			{
				in.close();
			}
			catch (java.io.IOException e)
			{
			throw new RuntimeException("Unexpected exception " + e.toString() );
			}
		}
	}

	public static String id()
	{
		return "IDL:demo/ami/MyException:1.0";
	}
	public static demo.ami.MyException read (final org.omg.CORBA.portable.InputStream in)
	{
		String id = in.read_string();
		if (!id.equals(id())) throw new org.omg.CORBA.MARSHAL("wrong id: " + id);
		final demo.ami.MyException result = new demo.ami.MyException(id);
		return result;
	}
	public static void write (final org.omg.CORBA.portable.OutputStream out, final demo.ami.MyException s)
	{
		out.write_string(id());
	}
}
