/*
This is a web scraping program to collect personal data from www.vgm.org.tw

This program let users to login as an adminstrator of the website through a web browser 
and collecting applicant's data.

The data which this program is meant to collect are not available online after 2017.

*/



package VGM.VgmDataCollector_2016;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.net.URL;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.util.HSSFColor;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.xssf.usermodel.XSSFCell;
import org.apache.poi.xssf.usermodel.XSSFRow;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.sun.javafx.application.PlatformImpl;

import javafx.embed.swing.JFXPanel;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.scene.web.WebEngine;
import javafx.scene.web.WebView;

public class Main {

	public static void main(String[] args) {
		
		MainFrame mainFrame = new MainFrame();
		mainFrame.setVisible(true);

	}

}


class MainFrame extends JFrame
{
	private static final long serialVersionUID = 1L;

	Browser browser = new Browser("http://www.vgm.org.tw/v3/modules/tinyd1/");
	
	XSSFWorkbook wb = new XSSFWorkbook();
	XSSFSheet individualSheet = wb.createSheet("個別梯報名資料");
	XSSFSheet groupSheet = wb.createSheet("包梯報名資料");
	
	final String excelFileName = "2016雙語營報名資料.xlsx";//name of excel file
	
	static private int ind_rowIndex = 1;
	static private int group_rowIndex = 1;
	
	public MainFrame()
	{
		setBounds(100, 100, 1200, 500);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setWorkBook(wb, individualSheet, groupSheet);
		add(browser, BorderLayout.CENTER);
		
		JButton btnStart = new JButton("After log in, press this button to start");
		btnStart.addActionListener(new ActionListener(){
			
			public void actionPerformed(ActionEvent e)
			{
				for (int id = 3930; id < 4204; id++)//can be adjusted
				{
					try
					{
						String idString = Integer.toString(id);
					
						String url = "http://www.vgm.org.tw/v3/modules/eguide/receipt.php?rvid=" + idString;
					
						URL link = new URL(url);
					
						String pageSourceCode = "";
							
						BufferedReader in
							= new BufferedReader(new InputStreamReader(link.openStream(), "big5"));
						
						String temp = "";
						
						while((temp = in.readLine()) != null)
						{
							pageSourceCode += temp;
							pageSourceCode += "\n";
						}
						
						processData(pageSourceCode);
						
					}
					catch(Exception exception){ }
				}
				
				try
				{
					FileOutputStream fileOut = new FileOutputStream(excelFileName);
					wb.write(fileOut);
					fileOut.flush();
					fileOut.close();
					wb.close();
				}catch(Exception exception){ }
				
				JOptionPane.showMessageDialog(MainFrame.this, "資料已匯入Excel檔");
				System.exit(0);
			}
		});
		
		btnStart.setBounds(256, 104, 87, 23);
		add(btnStart, BorderLayout.SOUTH);
	}
	
	
	public void processData(String sourceCode)
	{
		if(sourceCode.contains("我已仔細閱讀完畢"))
		{
			if(sourceCode.contains("包梯團契")) //group
				group_toExcel(group_dataManipulate(sourceCode));
			else individual_toExcel(individual_dataManipulate(sourceCode));
		}
	}
	
	public void setWorkBook(XSSFWorkbook wb, XSSFSheet individualSheet, XSSFSheet groupSheet)
	{
		Font titlefont = wb.createFont();
        titlefont.setColor(HSSFColor.BLACK.index);//顏色
        titlefont.setBoldweight(Font.BOLDWEIGHT_BOLD); //粗體

        CellStyle titleStyle = wb.createCellStyle();
        titleStyle.setFillForegroundColor(HSSFColor.LIGHT_GREEN.index);//填滿顏色
        titleStyle.setFillPattern(HSSFCellStyle.SOLID_FOREGROUND);
        titleStyle.setFont(titlefont);//設定字體
        titleStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);//水平置中
        titleStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);//垂直置中
        
      //設定框線
        titleStyle.setBorderBottom((short)1);
        titleStyle.setBorderTop((short)1);
        titleStyle.setBorderLeft((short)1);
        titleStyle.setBorderRight((short)1);
        titleStyle.setWrapText(true);//自動換行

        CellStyle normalStyle = wb.createCellStyle();
        normalStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);//水平置中
        normalStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);//垂直置中
        normalStyle.setBorderBottom((short)1);
        normalStyle.setBorderTop((short)1);
        normalStyle.setBorderLeft((short)1);
        normalStyle.setBorderRight((short)1);
        normalStyle.setWrapText(true);//自動換行
        
        
        XSSFRow ind_titlerow = individualSheet.createRow(0);
        
        XSSFCell [] ind_cells = new XSSFCell[40];//can be adjusted
        for(int i = 0; i< ind_cells.length; i++)
        {
        	ind_cells[i] = ind_titlerow.createCell(i);
        	ind_cells[i].setCellStyle(titleStyle);
        }
        
        ind_cells[0].setCellValue("報名ID編號");
        ind_cells[1].setCellValue("日期");
        ind_cells[2].setCellValue("電子郵件");
        ind_cells[3].setCellValue("我已仔細閱讀完畢");
        ind_cells[4].setCellValue("姓名");
        ind_cells[5].setCellValue("性別");
        ind_cells[6].setCellValue("就讀學校或職業");  
        ind_cells[7].setCellValue("科系或職稱");
        ind_cells[8].setCellValue("生日");
        ind_cells[9].setCellValue("身份証或護照");
        ind_cells[10].setCellValue("電話");
        ind_cells[11].setCellValue("手機");
        ind_cells[12].setCellValue("通訊地址");
        ind_cells[13].setCellValue("Facebook名稱");
        ind_cells[14].setCellValue("健康狀況");
        ind_cells[15].setCellValue("特殊疾病");
        ind_cells[16].setCellValue("若有特殊疾病請填");
        ind_cells[17].setCellValue("緊急連絡人");
        ind_cells[18].setCellValue("緊急連絡人與報名者的關係");
        ind_cells[19].setCellValue("緊急連絡電話");
        ind_cells[20].setCellValue("如何得知營隊資訊");
        ind_cells[21].setCellValue("續上題，其他方式");
        ind_cells[22].setCellValue("預計參加的梯數");
        ind_cells[23].setCellValue("可以參加的週次");
        ind_cells[24].setCellValue("是否已繳交報名費");
        ind_cells[25].setCellValue("T恤尺寸");
        ind_cells[26].setCellValue("職務安排調查(你可以勝任的職務)");
        ind_cells[27].setCellValue("個人恩賜調查");
        ind_cells[28].setCellValue("英語表達能力");
        ind_cells[29].setCellValue("英語理解能力");
        ind_cells[30].setCellValue("信仰狀況");
        ind_cells[31].setCellValue("所屬教會");
        ind_cells[32].setCellValue("鄉福經驗");
        ind_cells[33].setCellValue("曾經參與哪些鄉福營隊");
        ind_cells[34].setCellValue("曾經參與哪一年的什麼職位");
        ind_cells[35].setCellValue("曾經參與過其他營隊的什麼職務");
        ind_cells[36].setCellValue("輔導的得救見證");
        ind_cells[37].setCellValue("核對基本資料時間");
        ind_cells[38].setCellValue("哪一個時段您比較方便");
        ind_cells[39].setCellValue("備註");
        
        
        XSSFRow group_titlerow = groupSheet.createRow(0);
        
        XSSFCell [] group_cells = new XSSFCell[31];//can be adjusted
        for(int i = 0; i< group_cells.length; i++)
        {
        	group_cells[i] = group_titlerow.createCell(i);
        	group_cells[i].setCellStyle(titleStyle);
        }
        
        group_cells[0].setCellValue("報名ID編號");
        group_cells[1].setCellValue("日期");
        group_cells[2].setCellValue("電子郵件");
        group_cells[3].setCellValue("我已仔細閱讀完畢");
        group_cells[4].setCellValue("我確定");
        group_cells[5].setCellValue("包梯團契");
        group_cells[6].setCellValue("姓名");
        group_cells[7].setCellValue("性別");
        group_cells[8].setCellValue("就讀學校或職業");  
        group_cells[9].setCellValue("科系或職稱");
        group_cells[10].setCellValue("生日");
        group_cells[11].setCellValue("身份証或護照");
        group_cells[12].setCellValue("電話");
        group_cells[13].setCellValue("手機");
        group_cells[14].setCellValue("通訊地址");
        group_cells[15].setCellValue("Facebook名稱");
        group_cells[16].setCellValue("健康狀況");
        group_cells[17].setCellValue("特殊疾病");
        group_cells[18].setCellValue("若有特殊疾病請填");
        group_cells[19].setCellValue("緊急連絡人");
        group_cells[20].setCellValue("緊急連絡人與報名者的關係");
        group_cells[21].setCellValue("緊急連絡電話");
        group_cells[22].setCellValue("T恤尺寸");
        group_cells[23].setCellValue("個人恩賜調查");
        group_cells[24].setCellValue("信仰狀況");
        group_cells[25].setCellValue("所屬教會");
        group_cells[26].setCellValue("輔導的得救見證");
        group_cells[27].setCellValue("是否願意支援個別梯");
        group_cells[28].setCellValue("可以參加的個別梯週次");
        group_cells[29].setCellValue("可以在個別梯中擔任的職務");
        group_cells[30].setCellValue("備註");
	}
	
	
	public String [] individual_dataManipulate(String sourceCode)
	{
		String [] dataTitle = new String[40];
		int [] index_start = new int[40];
		int [] index_end = new int[40];
		String [] data = new String[40];
		
		dataTitle[0] = "報名ID編號";
        dataTitle[1] = "<th>日期";
        dataTitle[2] = "<th>電子郵件";
        dataTitle[3] = "我已仔細閱讀完畢";
        dataTitle[4] = "姓名";
        dataTitle[5] = "性別";
        dataTitle[6] = "就讀學校或職業";  
        dataTitle[7] = "科系或職稱"; 
        dataTitle[8] = "生日";
        dataTitle[9] = "身份証或護照";
        dataTitle[10] = "<th>電話";
        dataTitle[11] = "手機";
        dataTitle[12] = "通訊地址";
        dataTitle[13] = "Facebook名稱";
        dataTitle[14] = "健康狀況";
        dataTitle[15] = "特殊疾病<br>有氣喘、憂鬱、躁鬱、癲癇、<br>心血管疾病、半年內進行重大<br>手術等病史，請務必在下行填寫";
        dataTitle[16] = "----若有請填";
        dataTitle[17] = "緊急連絡人";
        dataTitle[18] = "與報名者的關係";
        dataTitle[19] = "緊急連絡電話";
        dataTitle[20] = "如何得知營隊資訊";
        dataTitle[21] = "續上題，其他方式";
        dataTitle[22] = "預計參加的梯數";
        dataTitle[23] = "可以參加的週次";
        dataTitle[24] = "繳交報名費";
        dataTitle[25] = "T恤尺寸";
        dataTitle[26] = "職務安排調查<br>你可以勝任的職務";
        dataTitle[27] = "個人恩賜調查";
        dataTitle[28] = "英語表達能力";
        dataTitle[29] = "英語理解能力";
        dataTitle[30] = "信仰狀況";
        dataTitle[31] = "所屬教會";
        dataTitle[32] = "鄉福經驗";
        dataTitle[33] = "曾經參與哪些鄉福營隊";
        dataTitle[34] = "曾經參與哪一年的什麼職位";
        dataTitle[35] = "曾經參與過其他營隊的什麼職務";
        dataTitle[36] = "輔導的得救見證<br>（300-500字，務必填寫）";
        dataTitle[37] = "若您第一次報名，<br>會與您核對基本資料，<br>請問你那一天方便";
        dataTitle[38] = "哪一個時段您比較方便";
        dataTitle[39] = "備註";
			
		index_start[0] = sourceCode.indexOf("報名ID編號", 25000)+ 15;
		index_end[0] = sourceCode.indexOf(" ", index_start[0]);
		data[0] = sourceCode.substring(index_start[0], index_end[0]);
		
		for(int i = 1; i<data.length; i++)
		{
			index_start[i] = sourceCode.indexOf(dataTitle[i], 25000) + dataTitle[i].length() + 9;
			index_end[i] = sourceCode.indexOf("<", index_start[i]);
			data[i] = sourceCode.substring(index_start[i], index_end[i]);
		}
		
		return data;
	}
	
	
	public void individual_toExcel(String [] data)
	{
		XSSFRow ind_row = individualSheet.createRow(ind_rowIndex);
		XSSFCell [] c = new XSSFCell[data.length];
		for(int i = 0; i<data.length; i++)
		{
			c[i] = ind_row.createCell(i);
			c[i].setCellValue(data[i]);
		}
		
		individualSheet.autoSizeColumn(ind_rowIndex);
		
		ind_rowIndex++;
	}
	
	
	
	public String [] group_dataManipulate(String sourceCode)
	{
		String [] dataTitle = new String[31];
		int [] index_start = new int[31];
		int [] index_end = new int[31];
		String [] data = new String[31];
		
		dataTitle[0] = "報名ID編號";
        dataTitle[1] = "<th>日期";
        dataTitle[2] = "<th>電子郵件";
        dataTitle[3] = "我已仔細閱讀完畢";
        dataTitle[4] = "我確定";
        dataTitle[5] = "包梯團契";
        dataTitle[6] = "姓名";
        dataTitle[7] = "性別";
        dataTitle[8] = "就讀學校或職業";  
        dataTitle[9] = "科系或職稱";
        dataTitle[10] = "生日";
        dataTitle[11] = "身份証或護照";
        dataTitle[12] = "<th>電話";
        dataTitle[13] = "手機";
        dataTitle[14] = "通訊地址";
        dataTitle[15] = "Facebook名稱";
        dataTitle[16] = "健康狀況";
        dataTitle[17] = "特殊疾病<br>有氣喘、憂鬱、躁鬱、癲癇、<br>心血管疾病、半年內進行重大<br>手術等病史，請務必在下行填寫";
        dataTitle[18] = "----若有請填";
        dataTitle[19] = "緊急連絡人";
        dataTitle[20] = "與報名者的關係";
        dataTitle[21] = "緊急連絡電話";
        dataTitle[22] = "T恤尺寸";
        dataTitle[23] = "個人恩賜調查";
        dataTitle[24] = "信仰狀況";
        dataTitle[25] = "所屬教會";
        dataTitle[26] = "輔導的得救見證<br>（300-500字，務必填寫）";
        dataTitle[27] = "是否願意支援個別梯";
        dataTitle[28] = "可以參加的個別梯週次";
        dataTitle[29] = "可以在個別梯中擔任的職務";
        dataTitle[30] = "備註";
        
        index_start[0] = sourceCode.indexOf("報名ID編號", 25000)+ 15;
		index_end[0] = sourceCode.indexOf(" ", index_start[0]);
		data[0] = sourceCode.substring(index_start[0], index_end[0]);
		
		for(int i = 1; i<data.length; i++)
		{
			index_start[i] = sourceCode.indexOf(dataTitle[i], 25000) + dataTitle[i].length() + 9;
			index_end[i] = sourceCode.indexOf("<", index_start[i]);
			data[i] = sourceCode.substring(index_start[i], index_end[i]);
		}
		return data;
	}
	
	
	public void group_toExcel(String [] data)
	{
		XSSFRow group_row = groupSheet.createRow(group_rowIndex);
		XSSFCell [] c = new XSSFCell[data.length];
		for(int i = 0; i<data.length; i++)
		{
			c[i] = group_row.createCell(i);
			c[i].setCellValue(data[i]);
		}
		
		groupSheet.autoSizeColumn(group_rowIndex);
		group_rowIndex++;
	}
}



class Browser extends JFXPanel {
	private static final long serialVersionUID = 1L;
	private WebEngine webEngine;

    public Browser(final String url) {
    	
       PlatformImpl.startup(new Runnable() {
           
    	   public void run() {
            	
                AnchorPane anchorPane = new AnchorPane();
                WebView web = new WebView();

                AnchorPane.setTopAnchor(web, 0.0);
                AnchorPane.setBottomAnchor(web, 0.0);
                AnchorPane.setLeftAnchor(web, 0.0);
                AnchorPane.setRightAnchor(web, 0.0);

                // Add WebView to AnchorPane
                anchorPane.getChildren().add(web);

                // Create Scene
                Scene scene = new Scene(anchorPane);

                // Obtain the webEngine to navigate
                webEngine = web.getEngine();
                webEngine.load(url);

                setScene(scene);
            }
        });
 
    }
    
    public WebEngine getWebEngine()
    {
    	return webEngine;
    }
}