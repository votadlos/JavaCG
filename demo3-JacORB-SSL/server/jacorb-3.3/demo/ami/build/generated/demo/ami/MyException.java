package demo.ami;

/**
 * Generated from IDL exception "MyException".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public final class MyException
	extends org.omg.CORBA.UserException
{
	/** Serial version UID. */
	private static final long serialVersionUID = 1L;
	public MyException()
	{
		super(demo.ami.MyExceptionHelper.id());
	}

	public MyException(String value)
	{
		super(value);
	}
}
