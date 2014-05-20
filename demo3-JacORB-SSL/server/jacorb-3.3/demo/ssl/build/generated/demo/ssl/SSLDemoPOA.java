package demo.ssl;


/**
 * Generated from IDL interface "SSLDemo".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:35 PM
 */

public abstract class SSLDemoPOA
	extends org.omg.PortableServer.Servant
	implements org.omg.CORBA.portable.InvokeHandler, demo.ssl.SSLDemoOperations
{
	static private final java.util.HashMap<String,Integer> m_opsHash = new java.util.HashMap<String,Integer>();
	static
	{
		m_opsHash.put ( "printCert", Integer.valueOf(0));
	}
	private String[] ids = {"IDL:demo/ssl/SSLDemo:1.0"};
	public demo.ssl.SSLDemo _this()
	{
		org.omg.CORBA.Object __o = _this_object() ;
		demo.ssl.SSLDemo __r = demo.ssl.SSLDemoHelper.narrow(__o);
		return __r;
	}
	public demo.ssl.SSLDemo _this(org.omg.CORBA.ORB orb)
	{
		org.omg.CORBA.Object __o = _this_object(orb) ;
		demo.ssl.SSLDemo __r = demo.ssl.SSLDemoHelper.narrow(__o);
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
			case 0: // printCert
			{
				_out = handler.createReply();
				printCert();
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
