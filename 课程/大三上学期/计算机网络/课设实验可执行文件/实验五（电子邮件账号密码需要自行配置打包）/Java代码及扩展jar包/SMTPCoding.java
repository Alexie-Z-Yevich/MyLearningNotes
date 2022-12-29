import java.util.*;
import javax.mail.*;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

public class SMTPCoding {

    public static void main(String[] args) throws Exception {
        String from = "1213791406@qq.com";                     // 配置发送邮箱
        String to = "yisanqian233@gmail.com";                  // 配置接收邮箱
        String subject = "test!!!!";                           // 邮件标题
        String body = "hello!!";                               // 邮件正文
        String smtpHost = "smtp.qq.com";                       // smtp协议使用方（这里是QQ）

        Properties props = new Properties();                   // 创建配置文件
        props.setProperty("mail.transport.protocol", "smtp");  // 设置协议类型
        props.setProperty("mail.smtp.host", smtpHost);         // 设置协议使用方
        props.setProperty("mail.smtp.auth", "true");           // 是否需要用户验证
        Session session = Session.getInstance(props);          // 作为邮件的容器
        session.setDebug(true);                                // 开启Bug模式

        Message msg = new MimeMessage(session);                // 创建要发送的信息，并将容器放入其中
        msg.setFrom(new InternetAddress(from));                // 设置发送方
        msg.setRecipients(Message.RecipientType.TO, InternetAddress.parse(to));  // 设置接收方
        msg.setSentDate(new Date());                           // 获取当前时间
        msg.setSubject(subject);                               // 设置标题
        msg.setText(body);                                     // 设置正文
        msg.saveChanges();                                     // 保存配置

        Transport transport = session.getTransport("smtp");  // 设置传输协议
        transport.connect(smtpHost, 25, from, "");  // 配置授权码/密码
        transport.sendMessage(msg, msg.getAllRecipients());    // 发送信息并获取收件人地址
        transport.close();
    }

}
