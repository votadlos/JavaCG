package demo.ami;


/**
 * Generated from IDL interface "AMI_AsyncServerHandler".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public abstract class AMI_AsyncServerHandlerPOA
	extends org.omg.PortableServer.Servant
	implements org.omg.CORBA.portable.InvokeHandler, demo.ami.AMI_AsyncServerHandlerOperations
{
	static private final java.util.HashMap<String,Integer> m_opsHash = new java.util.HashMap<String,Integer>();
	static
	{
		m_opsHash.put ( "op2", Integer.valueOf(0));
		m_opsHash.put ( "op2_excep", Integer.valueOf(1));
		m_opsHash.put ( "operation_excep", Integer.valueOf(2));
		m_opsHash.put ( "operation", Integer.valueOf(3));
	}
	private String[] ids = {"IDL:demo/ami/AMI_AsyncServerHandler:1.0","IDL:omg.org/Messaging/ReplyHandler:1.0"};
	public demo.ami.AMI_AsyncServerHandler _this()
	{
		org.omg.CORBA.Object __o = _this_object() ;
		demo.ami.AMI_AsyncServerHandler __r = demo.ami.AMI_AsyncServerHandlerHelper.narrow(__o);
		return __r;
	}
	public demo.ami.AMI_AsyncServerHandler _this(org.omg.CORBA.ORB orb)
	{
		org.omg.CORBA.Object __o = _this_object(orb) ;
		demo.ami.AMI_AsyncServerHandler __r = demo.ami.AMI_AsyncServerHandlerHelper.narrow(__o);
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
				int _arg0=_input.read_long();
				_out = handler.createReply();
				op2(_arg0);
				break;
			}
			case 1: // op2_excep
			{
				org.omg.Messaging.ExceptionHolder _arg0=(org.omg.Messaging.ExceptionHolder)((org.omg.CORBA_2_3.portable.InputStream)_input).read_value ("IDL:omg.org/Messaging/ExceptionHolder:1.0");
				_out = handler.createReply();
				op2_excep(_arg0);
				break;
			}
			case 2: // operation_excep
			{
				org.omg.Messaging.ExceptionHolder _arg0=(org.omg.Messaging.ExceptionHolder)((org.omg.CORBA_2_3.portable.InputStream)_input).read_value ("IDL:omg.org/Messaging/ExceptionHolder:1.0");
				_out = handler.createReply();
				operation_excep(_arg0);
				break;
			}
			case 3: // operation
			{
				int _arg0=_input.read_long();
				_out = handler.createReply();
				operation(_arg0);
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
