package demo.ssl;

/**
 * Generated from IDL interface "SSLDemo".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:35 PM
 */

public final class SSLDemoHolder	implements org.omg.CORBA.portable.Streamable{
	 public SSLDemo value;
	public SSLDemoHolder()
	{
	}
	public SSLDemoHolder (final SSLDemo initial)
	{
		value = initial;
	}
	public org.omg.CORBA.TypeCode _type()
	{
		return SSLDemoHelper.type();
	}
	public void _read (final org.omg.CORBA.portable.InputStream in)
	{
		value = SSLDemoHelper.read (in);
	}
	public void _write (final org.omg.CORBA.portable.OutputStream _out)
	{
		SSLDemoHelper.write (_out,value);
	}
}
