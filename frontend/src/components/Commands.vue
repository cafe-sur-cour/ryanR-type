<template>
  <div class="min-h-screen bg-gray-900 p-6">
    <div class="max-w-6xl mx-auto space-y-6">
      <!-- Header -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <div class="flex justify-between items-center">
          <div>
            <h1 class="text-3xl font-bold text-white">Server Commands</h1>
            <p class="text-gray-400 mt-1">Execute administrative commands</p>
          </div>
          <div class="flex space-x-3">
            <Button @click="refreshData" :disabled="loading" variant="default" class="flex items-center space-x-2 bg-blue-600 hover:bg-blue-700">
              <RefreshCwIcon class="h-4 w-4" :class="{ 'animate-spin': loading }" />
              <span>Refresh</span>
            </Button>
          </div>
        </div>
      </div>

      <!-- System Information -->
      <div class="grid grid-cols-1 md:grid-cols-4 gap-6">
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ServerIcon class="h-8 w-8 text-blue-400" />
            <div>
              <p class="text-sm text-gray-400">Server Status</p>
              <p :class="overview.serverStatus === 'Online' ? 'text-green-400' : 'text-red-400'" class="text-xl font-bold">
                {{ overview.serverStatus || 'Offline' }}
              </p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <UsersIcon class="h-8 w-8 text-green-400" />
            <div>
              <p class="text-sm text-gray-400">Connected Clients</p>
              <p class="text-2xl font-bold text-white">{{ overview.connectedClients || 0 }}</p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ClockIcon class="h-8 w-8 text-yellow-400" />
            <div>
              <p class="text-sm text-gray-400">Uptime</p>
              <p class="text-2xl font-bold text-white">{{ typeof overview.uptime === 'number' ? formatUptime(overview.uptime) : overview.uptime }}</p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ActivityIcon class="h-8 w-8 text-purple-400" />
            <div>
              <p class="text-sm text-gray-400">TPS</p>
              <p class="text-2xl font-bold text-white">{{ overview.tps || 0 }}</p>
            </div>
          </div>
        </div>
      </div>

      <!-- Overview -->
      <div class="grid grid-cols-1 md:grid-cols-4 gap-6">
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3 text-center">
            <GamepadIcon class="h-8 w-8 text-purple-400" />
            <div>
              <p class="text-sm text-gray-400 text-center">Active Lobbies</p>
              <p class="text-2xl font-bold text-white">{{ overview.activeLobbies || 0 }}</p>
            </div>
          </div>
          <div v-if="overview.lobbyDetails.length > 0" class="mt-4">
            <ul class="text-sm text-gray-300 space-y-1">
              <li v-for="lobby in overview.lobbyDetails" :key="lobby">{{ lobby.split(':')[0] }}</li>
            </ul>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3 text-center">
            <UsersIcon class="h-8 w-8 text-green-400" />
            <div>
              <p class="text-sm text-gray-400 text-center">Logged In Players</p>
              <p class="text-2xl font-bold text-white">{{ overview.loggedInPlayers || 0 }}</p>
            </div>
          </div>
          <div v-if="overview.playerDetails.length > 0" class="mt-4">
            <ul class="text-sm text-gray-300 space-y-1">
              <li v-for="player in overview.playerDetails" :key="player">{{ player }}</li>
            </ul>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3 text-center">
            <PlayIcon class="h-8 w-8 text-blue-400" />
            <div>
              <p class="text-sm text-gray-400 text-center">Players In Game</p>
              <p class="text-2xl font-bold text-white">{{ overview.playersInGame || 0 }}</p>
            </div>
          </div>
          <div v-if="overview.inGamePlayers.length > 0" class="mt-4">
            <ul class="text-sm text-gray-300 space-y-1">
              <li v-for="player in overview.inGamePlayers" :key="player">{{ player }}</li>
            </ul>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3 text-center">
            <BanIcon class="h-8 w-8 text-red-400" />
            <div>
              <p class="text-sm text-gray-400 text-center">Banned Players</p>
              <p class="text-2xl font-bold text-white">{{ overview.bannedPlayers?.length || 0 }}</p>
            </div>
          </div>
          <div v-if="overview.bannedPlayers && overview.bannedPlayers.length > 0" class="mt-4">
            <ul class="text-sm text-gray-300 space-y-1">
              <li v-for="player in overview.bannedPlayers" :key="player" class="text-red-400">{{ player }}</li>
            </ul>
          </div>
        </div>
      </div>

      <!-- Command Prompt -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <h2 class="text-xl font-bold text-white mb-4">Command Prompt</h2>
        <div class="space-y-4">
          <div class="flex space-x-2">
            <input
              v-model="command"
              @keydown.enter="executeCommand"
              type="text"
              placeholder="Enter command..."
              class="flex-1 bg-gray-700 border border-gray-600 rounded-lg px-4 py-2 text-white placeholder-gray-400 focus:outline-none focus:ring-2 focus:ring-indigo-500"
            />
            <Button @click="executeCommand" :disabled="!command.trim()" variant="default" class="bg-blue-600 hover:bg-blue-700">
              Execute
            </Button>
          </div>

          <!-- Suggestions -->
          <div v-if="allSuggestions.length > 0" class="bg-gray-700 rounded-lg p-4">
            <p class="text-sm text-gray-400 mb-2">Suggestions:</p>
            <div class="space-y-1">
              <button
                v-for="suggestion in allSuggestions"
                :key="suggestion"
                @click="command = suggestion"
                class="block w-full text-left text-gray-300 hover:text-white hover:bg-gray-600 px-2 py-1 rounded"
              >
                {{ suggestion }}
              </button>
            </div>
          </div>

          <!-- Output -->
          <div v-if="output" class="bg-gray-700 rounded-lg p-4">
            <p class="text-sm text-gray-400 mb-2">Output:</p>
            <p class="text-white">{{ output }}</p>
          </div>
        </div>
      </div>

      <!-- Auto-refresh indicator -->
      <div class="fixed bottom-0 left-1/2 transform -translate-x-1/2 text-gray-500 text-sm bg-gray-900 px-4 py-2 rounded-t">
        Auto-refreshing every 5 seconds • Last updated: {{ lastUpdate }}
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import Button from './ui/Button.vue'
import { GamepadIcon, UsersIcon, PlayIcon, RefreshCwIcon, BanIcon, ServerIcon, ClockIcon, ActivityIcon } from 'lucide-vue-next'

interface OverviewInfo {
  activeLobbies: number
  loggedInPlayers: number
  playersInGame: number
  lobbyDetails: string[]
  playerDetails: string[]
  inGamePlayers: string[]
  bannedPlayers: string[]
  serverStatus: string
  connectedClients: number
  uptime: string | number
  tps: number
}

interface Props {
  password: string
}

const props = defineProps<Props>()

const command = ref('')
const allSuggestions = ref<string[]>([])
const output = ref('')
const lastUpdate = ref('')
const loading = ref(false)
const overview = ref<OverviewInfo>({
  activeLobbies: 0,
  loggedInPlayers: 0,
  playersInGame: 0,
  lobbyDetails: [],
  playerDetails: [],
  inGamePlayers: [],
  bannedPlayers: [],
  serverStatus: 'Offline',
  connectedClients: 0,
  uptime: 'N/A' as string | number,
  tps: 0
})
const refreshInterval = ref<NodeJS.Timeout | null>(null)

const fetchOverview = async () => {
  loading.value = true
  try {
    const response = await fetch('/api/info', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })
    if (response.ok) {
      const data = await response.json()
      overview.value = {
        activeLobbies: data.activeLobbies || 0,
        loggedInPlayers: data.playerDetails ? data.playerDetails.length : 0,
        playersInGame: data.inGamePlayers ? data.inGamePlayers.length : 0,
        lobbyDetails: data.lobbyDetails || [],
        playerDetails: data.playerDetails || [],
        inGamePlayers: data.inGamePlayers || [],
        bannedPlayers: data.bannedPlayers || [],
        serverStatus: data.serverStatus || 'Offline',
        connectedClients: data.connectedClients || 0,
        uptime: data.uptime || 'N/A',
        tps: data.tps || 0
      }
      updateLastUpdate()
    }
  } catch (error) {
    console.error('Error fetching overview:', error)
  } finally {
    loading.value = false
  }
}

const fetchSuggestions = async () => {
  try {
    const response = await fetch('/api/commands/suggestions', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })
    if (response.ok) {
      const data = await response.json()
      allSuggestions.value = data.suggestions || []
    }
  } catch (error) {
    console.error('Error fetching suggestions:', error)
  }
}

const updateLastUpdate = () => {
  lastUpdate.value = new Date().toLocaleTimeString()
}

const formatUptime = (seconds?: number): string => {
  if (!seconds) return '0s'
  const hours = Math.floor(seconds / 3600)
  const minutes = Math.floor((seconds % 3600) / 60)
  const secs = seconds % 60

  if (hours > 0) return `${hours}h ${minutes}m ${secs}s`
  if (minutes > 0) return `${minutes}m ${secs}s`
  return `${secs}s`
}

const refreshData = () => {
  fetchOverview()
}

const executeCommand = async () => {
  if (!command.value.trim()) return

  try {
    const response = await fetch('/api/commands/execute', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${props.password}`
      },
      body: JSON.stringify({ command: command.value })
    })

    const data = await response.json()
    output.value = data.message || 'Command executed'

    if (response.ok) {
      // Refresh overview after command execution
      await fetchOverview()
    }
  } catch (error) {
    output.value = 'Error executing command'
    console.error('Error executing command:', error)
  }

  command.value = ''
}

onMounted(() => {
  fetchOverview()
  fetchSuggestions()
  refreshInterval.value = setInterval(fetchOverview, 5000)
})

onUnmounted(() => {
  if (refreshInterval.value) {
    clearInterval(refreshInterval.value)
  }
})
</script>