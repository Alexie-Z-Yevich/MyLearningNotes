import javax.mail.*;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.util.Date;
import java.util.Properties;
import java.util.Scanner;

public class MySMTPAndPOP {
    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);
        System.out.println("欢迎来到小艾同学的邮件系统！");
        while(true){
            System.out.println("请输入您想选择的服务：（0）发送邮件服务    （1）查看邮件服务");
            int a = sc.nextInt();
            SMTPAndPOP3 sap = new SMTPAndPOP3();
            if(a == 0 || a == 1) {
                System.out.println("请输入您的邮箱：");
                String from = sc.next();
                System.out.println("请输入您的邮箱密钥：");
                String pwd = sc.next();
                if(a == 0){
                    sap.STMP(from, pwd);
                }else {
                    sap.POP3(from, pwd);
                }
            }else {
                System.out.println("您输入的服务号错误！");
            }
        }
    }

}

class SMTPAndPOP3 {

    Scanner sc = new Scanner(System.in);

    public void STMP(String from, String pwd) throws Exception {

        System.out.println("请输入目的邮箱：");
        String to = sc.next();
        System.out.println("请输入邮件标题：");
        String subject = sc.next();
        System.out.println("请输入邮件正文：");
        String body = sc.next();

        Properties props = new Properties();                   // 创建配置文件
        props.setProperty("mail.transport.protocol", "smtp");  // 设置协议类型
        props.setProperty("mail.smtp.host", "smtp.qq.com");         // 设置协议使用方
        props.setProperty("mail.smtp.auth", "true");           // 是否需要用户验证
        Session session = Session.getInstance(props);          // 作为邮件的容器
        session.setDebug(false);                                // 开启Bug模式

        Message msg = new MimeMessage(session);                // 创建要发送的信息，并将容器放入其中
        msg.setFrom(new InternetAddress(from));                // 设置发送方
        msg.setRecipients(Message.RecipientType.TO, InternetAddress.parse(to));  // 设置接收方
        msg.setSentDate(new Date());                           // 获取当前时间
        msg.setSubject(subject);                               // 设置标题
        msg.setText(body);                                     // 设置正文
        msg.saveChanges();                                     // 保存配置
        Transport transport = session.getTransport("smtp");  // 设置传输协议
        transport.connect("smtp.qq.com", 25, from, pwd);  // 配置授权码/密码
        transport.sendMessage(msg, msg.getAllRecipients());    // 发送信息并获取收件人地址
        transport.close();

        System.out.println("\n发送成功!\n");
    }

    public void POP3(String from, String pwd) throws Exception {

        Properties props = new Properties();                     // 写入配置文件
        props.setProperty("mail.transport.protocol", "pop3");
        props.setProperty("mail.smtp.host", "pop.qq.com");

        Session session = Session.getInstance(props);            // 封装
        session.setDebug(false);

        Store store = session.getStore("pop3");                // store实现对配置文件的邮件协议上的读、写、监视、查找等操作
        store.connect("pop.qq.com", from, pwd);            // 连接

        Folder folder = store.getFolder("INBOX");             // 对store进行分级、权限管理
        folder.open(Folder.READ_WRITE);

        Message[] msgs = folder.getMessages();
        for(Message msg : msgs){
            String subject = msg.getSubject();                   // 获取信件主题
            Address address = msg.getFrom()[0];                     // 获取信件地址
            System.out.println("邮件主题：" + subject + "\n");
            System.out.println("邮件发件人：" + address + "\n");
            System.out.println("邮件的内容：");
            msg.writeTo(System.out);                             // 控制台打印邮件内容
        }

        folder.close(false);
        store.close();
    }
}
