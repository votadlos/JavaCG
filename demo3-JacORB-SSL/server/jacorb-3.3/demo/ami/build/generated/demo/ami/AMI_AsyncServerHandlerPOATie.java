package demo.ami;

import org.omg.PortableServer.POA;

/**
 * Generated from IDL interface "AMI_AsyncServerHandler".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public class AMI_AsyncServerHandlerPOATie
	extends AMI_AsyncServerHandlerPOA
{
	private AMI_AsyncServerHandlerOperations _delegate;

	private POA _poa;
	public AMI_AsyncServerHandlerPOATie(AMI_AsyncServerHandlerOperations delegate)
	{
		_delegate = delegate;
	}
	public AMI_AsyncServerHandlerPOATie(AMI_AsyncServerHandlerOperations delegate, POA poa)
	{
		_delegate = delegate;
		_poa = poa;
	}
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
	public AMI_AsyncServerHandlerOperations _delegate()
	{
		return _delegate;
	}
	public void _delegate(AMI_AsyncServerHandlerOperations delegate)
	{
		_delegate = delegate;
	}
	public POA _default_POA()
	{
		if (_poa != null)
		{
			return _poa;
		}
		return super._default_POA();
	}
	public void op2(int ami_return_val)
	{
_delegate.op2(ami_return_val);
	}

	public void op2_excep(org.omg.Messaging.ExceptionHolder excep_holder)
	{
_delegate.op2_excep(excep_holder);
	}

	public void operation_excep(org.omg.Messaging.ExceptionHolder excep_holder)
	{
_delegate.operation_excep(excep_holder);
	}

	public void operation(int ami_return_val)
	{
_delegate.operation(ami_return_val);
	}

}
