<template>
  <div class="min-h-screen bg-gray-900 p-4">
    <div class="max-w-6xl mx-auto space-y-6">
      <!-- Header -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <div class="flex justify-between items-center">
          <div>
            <h1 class="text-3xl font-bold text-white">R-Type Server Monitor</h1>
            <p class="text-gray-400 mt-1">Real-time server statistics and management</p>
          </div>
          <div class="flex space-x-3">
            <Button @click="refreshData" :disabled="loading" variant="outline" class="flex items-center space-x-2">
              <RefreshCwIcon class="h-4 w-4" :class="{ 'animate-spin': loading }" />
              <span>Refresh</span>
            </Button>
            <Button @click="$emit('logout')" variant="destructive">
              Logout
            </Button>
          </div>
        </div>
      </div>

      <!-- Status Overview -->
      <div class="grid grid-cols-1 md:grid-cols-4 gap-6">
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ServerIcon class="h-8 w-8 text-blue-400" />
            <div>
              <p class="text-sm text-gray-400">Server Status</p>
              <p :class="serverInfo.connectedClients !== undefined ? 'text-green-400' : 'text-red-400'" class="text-xl font-bold">
                {{ serverInfo.connectedClients !== undefined ? 'Online' : 'Offline' }}
              </p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <UsersIcon class="h-8 w-8 text-green-400" />
            <div>
              <p class="text-sm text-gray-400">Connected Clients</p>
              <p class="text-2xl font-bold text-white">{{ serverInfo.connectedClients || 0 }}</p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <GamepadIcon class="h-8 w-8 text-purple-400" />
            <div>
              <p class="text-sm text-gray-400">Active Lobbies</p>
              <p class="text-2xl font-bold text-white">{{ serverInfo.activeLobbies || 0 }}</p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ClockIcon class="h-8 w-8 text-yellow-400" />
            <div>
              <p class="text-sm text-gray-400">Uptime</p>
              <p class="text-2xl font-bold text-white">{{ formatUptime(serverInfo.uptime) }}</p>
            </div>
          </div>
        </div>
      </div>

      <!-- Detailed Information -->
      <div class="grid grid-cols-1 lg:grid-cols-2 gap-6">
        <!-- Lobby Details -->
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
            <GamepadIcon class="h-5 w-5 text-purple-400" />
            <span>Lobby Details</span>
          </h2>
          <div v-if="serverInfo.lobbyDetails && serverInfo.lobbyDetails.length > 0" class="space-y-2">
            <div v-for="(lobby, index) in serverInfo.lobbyDetails" :key="index"
                 class="bg-gray-700 rounded p-3 flex justify-between items-center">
              <span class="text-gray-300">{{ lobby }}</span>
              <span class="text-sm text-gray-500">Active</span>
            </div>
          </div>
          <div v-else class="text-gray-500 text-center py-4">
            No active lobbies
          </div>
        </div>

        <!-- Player Details -->
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
            <UsersIcon class="h-5 w-5 text-green-400" />
            <span>Connected Players</span>
          </h2>
          <div v-if="serverInfo.playerDetails && serverInfo.playerDetails.length > 0" class="space-y-2">
            <div v-for="(player, index) in serverInfo.playerDetails" :key="index"
                 class="bg-gray-700 rounded p-3 flex justify-between items-center">
              <span class="text-gray-300">{{ player }}</span>
              <div class="w-2 h-2 bg-green-400 rounded-full"></div>
            </div>
          </div>
          <div v-else class="text-gray-500 text-center py-4">
            No players connected
          </div>
        </div>
      </div>

      <!-- System Information -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
          <InfoIcon class="h-5 w-5 text-blue-400" />
          <span>System Information</span>
        </h2>
        <div class="grid grid-cols-1 md:grid-cols-3 gap-4">
          <div class="bg-gray-700 rounded p-4">
            <p class="text-sm text-gray-400">Total Players</p>
            <p class="text-2xl font-bold text-white">{{ serverInfo.totalPlayers || 0 }}</p>
          </div>
          <div class="bg-gray-700 rounded p-4">
            <p class="text-sm text-gray-400">HTTP Port</p>
            <p class="text-2xl font-bold text-white">5173</p>
          </div>
          <div class="bg-gray-700 rounded p-4">
            <p class="text-sm text-gray-400">Game Port</p>
            <p class="text-2xl font-bold text-white">4242</p>
          </div>
        </div>
      </div>

      <!-- Auto-refresh indicator -->
      <div class="text-center text-gray-500 text-sm">
        Auto-refreshing every 2 seconds • Last updated: {{ lastUpdate }}
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import Button from './ui/Button.vue'
import { RefreshCwIcon, ServerIcon, UsersIcon, GamepadIcon, ClockIcon, InfoIcon } from 'lucide-vue-next'

interface Props {
  password: string
}

interface ServerInfo {
  connectedClients?: number
  uptime?: number
  activeLobbies?: number
  totalPlayers?: number
  lobbyDetails?: string[]
  playerDetails?: string[]
}

const props = defineProps<Props>()

const emit = defineEmits<{
  logout: []
}>()

const serverInfo = ref<ServerInfo>({})
const loading = ref(false)
const lastUpdate = ref('')
const autoRefreshInterval = ref<NodeJS.Timeout | null>(null)

const formatUptime = (seconds?: number): string => {
  if (!seconds) return '0s'
  const hours = Math.floor(seconds / 3600)
  const minutes = Math.floor((seconds % 3600) / 60)
  const secs = seconds % 60

  if (hours > 0) return `${hours}h ${minutes}m ${secs}s`
  if (minutes > 0) return `${minutes}m ${secs}s`
  return `${secs}s`
}

const updateLastUpdate = () => {
  lastUpdate.value = new Date().toLocaleTimeString()
}

const fetchServerInfo = async () => {
  loading.value = true
  try {
    const response = await fetch('/api/info', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })

    if (response.ok) {
      serverInfo.value = await response.json()
      updateLastUpdate()
    } else if (response.status === 401) {
      emit('logout')
    } else {
      console.error('Failed to fetch server info')
    }
  } catch (error) {
    console.error('Error fetching server info:', error)
  } finally {
    loading.value = false
  }
}

const refreshData = () => {
  fetchServerInfo()
}

const startAutoRefresh = () => {
  autoRefreshInterval.value = setInterval(() => {
    fetchServerInfo()
  }, 2000)
}

const stopAutoRefresh = () => {
  if (autoRefreshInterval.value) {
    clearInterval(autoRefreshInterval.value)
    autoRefreshInterval.value = null
  }
}

onMounted(() => {
  fetchServerInfo()
  startAutoRefresh()
})

onUnmounted(() => {
  stopAutoRefresh()
})
</script>

<style scoped>
</style>