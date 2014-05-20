package demo.ami;


/**
 * Generated from IDL interface "AMI_AsyncServerHandler".
 *
 * @author JacORB IDL compiler V 3.3
 * @version generated at Apr 4, 2014 9:31:22 PM
 */

public interface AMI_AsyncServerHandlerOperations
	extends org.omg.Messaging.ReplyHandlerOperations
{
	/* constants */
	/* operations  */
	void operation(int ami_return_val);
	void operation_excep(org.omg.Messaging.ExceptionHolder excep_holder);
	void op2(int ami_return_val);
	void op2_excep(org.omg.Messaging.ExceptionHolder excep_holder);
}
