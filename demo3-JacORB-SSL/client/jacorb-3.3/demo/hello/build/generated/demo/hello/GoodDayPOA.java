package demo.hello;


/**
 * Generated from IDL interface "GoodDay".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 10:20:27 PM
 */

public abstract class GoodDayPOA
	extends org.omg.PortableServer.Servant
	implements org.omg.CORBA.portable.InvokeHandler, demo.hello.GoodDayOperations
{
	static private final java.util.HashMap<String,Integer> m_opsHash = new java.util.HashMap<String,Integer>();
	static
	{
		m_opsHash.put ( "hello_simple", Integer.valueOf(0));
		m_opsHash.put ( "hello_wide", Integer.valueOf(1));
	}
	private String[] ids = {"IDL:demo/hello/GoodDay:1.0"};
	public demo.hello.GoodDay _this()
	{
		org.omg.CORBA.Object __o = _this_object() ;
		demo.hello.GoodDay __r = demo.hello.GoodDayHelper.narrow(__o);
		return __r;
	}
	public demo.hello.GoodDay _this(org.omg.CORBA.ORB orb)
	{
		org.omg.CORBA.Object __o = _this_object(orb) ;
		demo.hello.GoodDay __r = demo.hello.GoodDayHelper.narrow(__o);
		return __r;
	}
	public org.omg.CORBA.portable.OutputStream _invoke(String method, org.omg.CORBA.portable.InputStream _input, org.omg.CORBA.portable.ResponseHandler handler)
		throws org.omg.CORBA.SystemException
	{
		org.omg.CORBA.portable.OutputStream _out = null;
		// do something
		// quick lookup of operation
		java.lang.Integer opsIndex = (java.lang.Integer)m_opsHash.get ( method );
		if ( null == opsIndex )
			throw new org.omg.CORBA.BAD_OPERATION(method + " not found");
		switch ( opsIndex.intValue() )
		{
			case 0: // hello_simple
			{
				_out = handler.createReply();
				java.lang.String tmpResult1 = hello_simple();
_out.write_string( tmpResult1 );
				break;
			}
			case 1: // hello_wide
			{
				java.lang.String _arg0=_input.read_wstring();
				_out = handler.createReply();
				java.lang.String tmpResult2 = hello_wide(_arg0);
_out.write_wstring( tmpResult2 );
				break;
			}
		}
		return _out;
	}

	public String[] _all_interfaces(org.omg.PortableServer.POA poa, byte[] obj_id)
	{
		return ids;
	}
}
