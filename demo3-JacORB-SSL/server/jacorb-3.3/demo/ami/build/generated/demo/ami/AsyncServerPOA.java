package demo.ami;


/**
 * Generated from IDL interface "AsyncServer".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public abstract class AsyncServerPOA
	extends org.omg.PortableServer.Servant
	implements org.omg.CORBA.portable.InvokeHandler, demo.ami.AsyncServerOperations
{
	static private final java.util.HashMap<String,Integer> m_opsHash = new java.util.HashMap<String,Integer>();
	static
	{
		m_opsHash.put ( "op2", Integer.valueOf(0));
		m_opsHash.put ( "operation", Integer.valueOf(1));
	}
	private String[] ids = {"IDL:demo/ami/AsyncServer:1.0"};
	public demo.ami.AsyncServer _this()
	{
		org.omg.CORBA.Object __o = _this_object() ;
		demo.ami.AsyncServer __r = demo.ami.AsyncServerHelper.narrow(__o);
		return __r;
	}
	public demo.ami.AsyncServer _this(org.omg.CORBA.ORB orb)
	{
		org.omg.CORBA.Object __o = _this_object(orb) ;
		demo.ami.AsyncServer __r = demo.ami.AsyncServerHelper.narrow(__o);
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
			case 0: // op2
			{
			try
			{
				int _arg0=_input.read_long();
				_out = handler.createReply();
				_out.write_long(op2(_arg0));
			}
			catch(demo.ami.MyException _ex0)
			{
				_out = handler.createExceptionReply();
				demo.ami.MyExceptionHelper.write(_out, _ex0);
			}
				break;
			}
			case 1: // operation
			{
				int _arg0=_input.read_long();
				int _arg1=_input.read_long();
				_out = handler.createReply();
				_out.write_long(operation(_arg0,_arg1));
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
