package demo.ami;

import org.omg.PortableServer.POA;

/**
 * Generated from IDL interface "AsyncServer".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public class AsyncServerPOATie
	extends AsyncServerPOA
{
	private AsyncServerOperations _delegate;

	private POA _poa;
	public AsyncServerPOATie(AsyncServerOperations delegate)
	{
		_delegate = delegate;
	}
	public AsyncServerPOATie(AsyncServerOperations delegate, POA poa)
	{
		_delegate = delegate;
		_poa = poa;
	}
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
	public AsyncServerOperations _delegate()
	{
		return _delegate;
	}
	public void _delegate(AsyncServerOperations delegate)
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
	public int op2(int a) throws demo.ami.MyException
	{
		return _delegate.op2(a);
	}

	public int operation(int a, int b)
	{
		return _delegate.operation(a,b);
	}

}
