package demo.ami;

/**
 * Generated from IDL exception "MyException".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public final class MyExceptionHolder
	implements org.omg.CORBA.portable.Streamable
{
	public demo.ami.MyException value;

	public MyExceptionHolder ()
	{
	}
	public MyExceptionHolder(final demo.ami.MyException initial)
	{
		value = initial;
	}
	public org.omg.CORBA.TypeCode _type ()
	{
		return demo.ami.MyExceptionHelper.type ();
	}
	public void _read(final org.omg.CORBA.portable.InputStream _in)
	{
		value = demo.ami.MyExceptionHelper.read(_in);
	}
	public void _write(final org.omg.CORBA.portable.OutputStream _out)
	{
		demo.ami.MyExceptionHelper.write(_out, value);
	}
}
