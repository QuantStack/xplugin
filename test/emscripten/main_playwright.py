import playwright
import socket
import threading

from contextlib import closing, contextmanager
from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import os
import asyncio
import sys

from playwright.async_api import async_playwright

# dir of this file
content_dir = Path(os.path.dirname(os.path.abspath(__file__)))/"content"
js_utils_file = Path(os.path.dirname(os.path.abspath(__file__))) / "js_utils.js"


def find_free_port():
    with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as s:
        s.bind(("", 0))
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        return s.getsockname()[1]


def start_server(work_dir, port):
    class Handler(SimpleHTTPRequestHandler):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, directory=work_dir, **kwargs)

        def log_message(self, fmt, *args):
            return

    httpd = HTTPServer(("127.0.0.1", port), Handler)

    thread = threading.Thread(target=httpd.serve_forever)
    thread.start()
    return thread, httpd


@contextmanager
def server_context(work_dir, port):
    thread, server = start_server(work_dir=work_dir, port=port)
    try:
        yield server, f"http://127.0.0.1:{port}"
    finally:
        server.shutdown()
        thread.join()



headless = True


async def playwright_run(page_url):
    async with async_playwright() as p:
        browser = await p.chromium.launch(headless=headless)

        print("new page")
        page = await browser.new_page()

        print("goto")
        await page.goto(page_url)
        # n min = n_min * 60 * 1000 ms
        n_min = 4
        page.set_default_timeout(n_min * 60 * 1000)

        async def handle_console(msg):
            txt = str(msg)
            print(txt)
        print("on console")
        page.on("console", handle_console)

        with open(js_utils_file) as f:
            js = f.read()

        status = await page.evaluate(js)
        return status




if __name__ == '__main__':
    print("Playwright test")
    port = find_free_port()
    with server_context(work_dir=content_dir, port=port) as (server, url):
        print(f"Server started at {url}")
        
        page_url = f"{url}/testpage.html"
        return_code = asyncio.run(playwright_run(page_url=page_url))
        sys.exit(return_code)
