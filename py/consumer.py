import pynng
import cbor2
import signal
import asyncio


def quit(signum, frame):
    print("Interrupted")
    exit(0)


# def main():
#     with pynng.Pull0(dial="ipc:///tmp/nng_cbor_example.ipc") as sock:
#         print("Consumer started, connect to ipc:///tmp/nng_cbor_example.ipc")

#         signal.signal(signal.SIGINT, quit)

#         while True:
#             try:
#                 msg = sock.recv()

#                 # Deserialize the message
#                 data = cbor2.loads(msg)

#                 print(f"Received: {data}")
#             except Exception as e:
#                 print(f"Error: {e}")
#                 break
#             except KeyboardInterrupt:
#                 print("Interrupted")
#                 break


async def main():
    with pynng.Pull0(listen="ipc:///tmp/nng_cbor_example.ipc") as sock:
        print("Consumer started, connect to ipc:///tmp/nng_cbor_example.ipc")

        while True:
            try:
                msg = await sock.arecv()

                # Deserialize the message
                data = cbor2.loads(msg)

                print(f"Received: {data}")
            except pynng.Timeout:
                print("Timeout")
                break
            except Exception as e:
                print(f"Error: {e}")
                break


if __name__ == "__main__":
    asyncio.run(main())
