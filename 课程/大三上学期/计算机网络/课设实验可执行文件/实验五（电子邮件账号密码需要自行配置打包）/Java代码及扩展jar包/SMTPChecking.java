import javax.mail.*;
import java.util.*;
// atqumjkwtsafvbpv
public class SMTPChecking {

    public static void main(String[] args) throws Exception {
        String popserver = "pop.qq.com";                         // POP协议使用方
        String protocol = "pop3";                                // 协议
        String username = "1213791406@qq.com";                   // 发送方邮箱
        String password = "";                    // 授权密钥

        Properties props = new Properties();                     // 写入配置文件
        props.setProperty("mail.transport.protocol", protocol);
        props.setProperty("mail.smtp.host", popserver);

        Session session = Session.getInstance(props);            // 封装
        session.setDebug(false);

        Store store = session.getStore(protocol);                // store实现对配置文件的邮件协议上的读、写、监视、查找等操作
        store.connect(popserver, username, password);            // 连接

        Folder folder = store.getFolder("INBOX");             // 对store进行分级、权限管理
        folder.open(Folder.READ_WRITE);

        Message[] msgs = folder.getMessages();
        for(Message msg : msgs){
            String subject = msg.getSubject();                   // 获取信件主题
            Address from = msg.getFrom()[0];                     // 获取信件地址
            System.out.println("邮件主题：" + subject + "\n");
            System.out.println("邮件发件人：" + from + "\n");
            System.out.println("邮件的内容：");
            msg.writeTo(System.out);                             // 控制台打印邮件内容
        }

        folder.close(false);
        store.close();
    }
}
