from splinter import Browser
browser = Browser()
screenshot_path = browser.screenshot('D:\your_screenshot1.png')
browser.fill('wd', 'splinter - python acceptance testing for web applications')
screenshot_path = browser.screenshot('D:\your_screenshot2.png')
button = browser.find_by_xpath('//input[@type="submit"]').click()
if browser.is_text_present('splinter.readthedocs'):
    print("Yes, the official website was found!")
else:
    print("No, it wasn't found... We need to improve our SEO techniques")

browser.visit('https://renhardly:lxw0325@github.com/apache/incubator-echarts')

screenshot_path = browser.screenshot('D:\your_screenshot3.png')
#browser.quit()



