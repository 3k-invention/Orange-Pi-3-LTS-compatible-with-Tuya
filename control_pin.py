# Comment from GITHUB
# comment from HOST
import subprocess


def set_pin(pin_number, desired_state):
    """
    Sets the specified GPIO pin to the desired state.

    Args:
        pin_number (int): The GPIO pin number to set.
        desired_state (int): The state to set the pin to, either 0 or 1.

    Raises:
        Exception: If the script execution returns a non-zero exit code,
                    indicating an error.
    """
    result = subprocess.run(
        ['set_GPIO.sh', str(pin_number), str(desired_state)],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    if result.returncode != 0:
        raise Exception(
            f"Error executing script: {result.stderr.decode('utf-8')}"
        )
