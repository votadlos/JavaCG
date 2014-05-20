package demo.ami;

/**
 * Generated from IDL interface "AsyncServer".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public final class AsyncServerHolder	implements org.omg.CORBA.portable.Streamable{
	 public AsyncServer value;
	public AsyncServerHolder()
	{
	}
	public AsyncServerHolder (final AsyncServer initial)
	{
		value = initial;
	}
	public org.omg.CORBA.TypeCode _type()
	{
		return AsyncServerHelper.type();
	}
	public void _read (final org.omg.CORBA.portable.InputStream in)
	{
		value = AsyncServerHelper.read (in);
	}
	public void _write (final org.omg.CORBA.portable.OutputStream _out)
	{
		AsyncServerHelper.write (_out,value);
	}
}
