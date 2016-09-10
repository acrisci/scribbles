#!/usr/bin/env python3

import time
from flask import Flask, request
from selenium import webdriver
from selenium.webdriver.chrome.webdriver import WebDriver

WEB_PORT = 7777

chrome_binary = '/home/tonyctl/projects/cef/chromium_git/chromium/src/out/Debug_GN_x64/cefclient'
chrome_options = webdriver.ChromeOptions()
chrome_options.binary_location = chrome_binary
driver = WebDriver(executable_path='./chromedriver', desired_capabilities={}, chrome_options=chrome_options)

app = Flask(__name__)
app.debug = True

@app.route('/sendkeys', methods=['POST'])
def send_keys():
    keys = request.data.decode()
    print(driver.window_handles)
    driver.find_element_by_css_selector('body').send_keys(keys)

app.run(host='127.0.0.1', port=WEB_PORT)
