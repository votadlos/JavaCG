package demo.ssl;

import org.omg.PortableServer.POA;

/**
 * Generated from IDL interface "SSLDemo".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:35 PM
 */

public class SSLDemoPOATie
	extends SSLDemoPOA
{
	private SSLDemoOperations _delegate;

	private POA _poa;
	public SSLDemoPOATie(SSLDemoOperations delegate)
	{
		_delegate = delegate;
	}
	public SSLDemoPOATie(SSLDemoOperations delegate, POA poa)
	{
		_delegate = delegate;
		_poa = poa;
	}
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
	public SSLDemoOperations _delegate()
	{
		return _delegate;
	}
	public void _delegate(SSLDemoOperations delegate)
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
	public void printCert()
	{
_delegate.printCert();
	}

}
